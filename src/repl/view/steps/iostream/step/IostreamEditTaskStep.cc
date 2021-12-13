#include "IostreamStep.h"
#include "repl/view/steps/ISmallStepFactory.h"
#include "repl/view/steps/TaskContext.h"
#include "repl/view/steps/TaskInitializerSmallStep.h"
#include "repl/view/steps/iostream/IostreamStrings.h"

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
  io_facility_->Print(IostreamStrings::kYouAreGoingToEdit);
  const auto &to_edit = *(task_wrapper_.get());
  io_facility_->Print(IostreamStrings::ShowTask(to_edit));
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

  io_facility_->Print(IostreamStrings::ProceedTo("edit"));

  std::string input = io_facility_->GetLine();
  auto confirm = validator_->ParseConfirmation(input);
  if (!confirm) {
    io_facility_->Print(IostreamStrings::kOkayITreatItAsNo);
    return StepResult{ConfirmationResult::kNo,
                      sub_context.GetTaskBuilder().GetTask()};
  }
  return {*confirm, sub_context.GetTaskBuilder().GetTask()};
}