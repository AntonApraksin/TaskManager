#include <sstream>

#include "IostreamStep.h"
#include "repl/view/steps/ISmallStepFactory.h"
#include "repl/view/steps/TaskContext.h"
#include "repl/view/steps/TaskInitializerSmallStep.h"
#include "repl/view/steps/iostream/IostreamGeneralFunctional.h"

IostreamEditTaskStep::IostreamEditTaskStep(
    const std::shared_ptr<IIoFacility> &io_facility,
    const std::shared_ptr<ISmallStepFactory> &state_factory,
    const std::shared_ptr<IValidator> &validator, TaskWrapperRef task_wrapper)
    : IEditTaskStep(task_wrapper),
      IostreamStep(io_facility),
      IostreamWithSmallStepStep(state_factory),
      IostreamWithValidatorStep(validator) {}

StepResult IostreamEditTaskStep::Run() {
  std::stringstream ss;
  ss << "You are going to edit:\n";
  io_facility_->Print(ss.str());
  ss.str("");
  const auto &to_edit = *(task_wrapper_.get());
  ShowTask(*io_facility_, to_edit);
  ss << "  it has " << task_wrapper_.get().ShowStorage().size()
            << " children.\n";
  io_facility_->Print(ss.str());
  ss.str("");
  TaskContext sub_context;
  sub_context.PushState(std::make_shared<DefaultTaskInitializerSmallStep>(
      TaskBuilder{/*.title =*/to_edit.GetTitle(),
                  /*.date_ =*/to_edit.GetDueDate(),
                  /*.priority =*/to_edit.GetPriority(),
                  /*.state =*/to_edit.GetState()}));
  sub_context.PushState(
      small_step_factory_->GetREPLState(IostreamSmallStepEnum::kReadTitle));
  sub_context.PushState(
      small_step_factory_->GetREPLState(IostreamSmallStepEnum::kReadDate));
  sub_context.PushState(
      small_step_factory_->GetREPLState(IostreamSmallStepEnum::kReadPriority));
  sub_context.PushState(
      small_step_factory_->GetREPLState(IostreamSmallStepEnum::kReadState));
  sub_context.Run();

  ss << "Proceed to edit? [Y/n]: ";
  io_facility_->Print(ss.str());
  ss.str("");

  std::string input = io_facility_->GetLine();
  auto confirm = validator_->ParseConfirmation(input);
  if (!confirm) {
    ss << "Okay, I treat it as no\n";
    io_facility_->Print(ss.str());
    ss.str("");
    return StepResult{ConfirmationResult::kNo,
                      sub_context.GetTaskBuilder().GetTask()};
  }
  return {*confirm, sub_context.GetTaskBuilder().GetTask()};
}