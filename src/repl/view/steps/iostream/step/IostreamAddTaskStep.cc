#include <sstream>

#include "IostreamStep.h"
#include "repl/view/steps/ISmallStepFactory.h"
#include "repl/view/steps/TaskContext.h"
#include "repl/view/steps/TaskInitializerSmallStep.h"
#include "repl/view/steps/iostream/IostreamGeneralFunctional.h"

IostreamAddTaskStep::IostreamAddTaskStep(
    const std::shared_ptr<IIoFacility>& io_facility,
    const std::shared_ptr<ISmallStepFactory>& small_step_factory,
    const std::shared_ptr<IValidator>& validator)
    : IostreamStep(io_facility),
      IostreamWithSmallStepStep(small_step_factory),
      IostreamWithValidatorStep(validator) {}

StepResult IostreamAddTaskStep::Run() {
  std::stringstream ss;
  TaskContext sub_context;
  if (task_) {
    ss << "Add subtask to:\n";
    io_facility_->Print(ss.str());
    ss.str("");
    ShowTask(*io_facility_, *task_);
    sub_context.PushState(std::make_shared<DefaultTaskInitializerSmallStep>(
        TaskBuilder{/*.title = */ std::nullopt,
                    /*.date_ =*/task_->GetDueDate(),
                    /*.priority =*/task_->GetPriority(),
                    /*.state =*/task_->GetState()}));
    task_.reset();
  } else {
    sub_context.PushState(
        std::make_shared<DefaultTaskInitializerSmallStep>(TaskBuilder{
            /*.title =*/std::nullopt,
            /*.date_ =*/std::chrono::system_clock::now(),  // TODO: give other
                                                           // default value
            /*.priority =*/Task::Priority::kLow,
            /*.state =*/Task::State::kUncompleted}));
  }

  sub_context.PushState(
      small_step_factory_->GetREPLState(IostreamSmallStepEnum::kReadTitle));
  sub_context.PushState(
      small_step_factory_->GetREPLState(IostreamSmallStepEnum::kReadDate));
  sub_context.PushState(
      small_step_factory_->GetREPLState(IostreamSmallStepEnum::kReadPriority));
  sub_context.PushState(
      small_step_factory_->GetREPLState(IostreamSmallStepEnum::kReadState));
  sub_context.Run();

  ss << "Proceed to add? [Y/n]: ";
  io_facility_->Print(ss.str());
  ss.str("");
  std::string input = io_facility_->GetLine();
  auto confirm = validator_->ParseConfirmation(input);
  if (!confirm) {
    ss << "Okay, i treat it as no\n";
    io_facility_->Print(ss.str());
    ss.str("");
    return StepResult{ConfirmationResult::kNo,
                      sub_context.GetTaskBuilder().GetTask()};
  }
  return {*confirm, sub_context.GetTaskBuilder().GetTask()};
}