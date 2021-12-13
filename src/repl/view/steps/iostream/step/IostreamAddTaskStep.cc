#include "IostreamStep.h"
#include "repl/view/steps/ISmallStepFactory.h"
#include "repl/view/steps/TaskContext.h"
#include "repl/view/steps/TaskInitializerSmallStep.h"
#include "repl/view/steps/iostream/IostreamStrings.h"

IostreamAddTaskStep::IostreamAddTaskStep(
    const std::shared_ptr<IIoFacility>& io_facility,
    const std::shared_ptr<ISmallStepFactory>& small_step_factory,
    const std::shared_ptr<IValidator>& validator)
    : IostreamStep(io_facility),
      IostreamWithSmallStepStep(small_step_factory),
      IostreamWithValidatorStep(validator) {}

StepResult IostreamAddTaskStep::Run() {
  TaskContext sub_context;
  if (task_) {
    io_facility_->Print(IostreamStrings::kAddSubtaskTo);
    io_facility_->Print(IostreamStrings::ShowTask(*task_));
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

  io_facility_->Print(IostreamStrings::ProceedTo("add"));
  std::string input = io_facility_->GetLine();
  auto confirm = validator_->ParseConfirmation(input);
  if (!confirm) {
    io_facility_->Print(IostreamStrings::kOkayITreatItAsNo);
    return StepResult{ConfirmationResult::kNo,
                      sub_context.GetTaskBuilder().GetTask()};
  }
  return {*confirm, sub_context.GetTaskBuilder().GetTask()};
}