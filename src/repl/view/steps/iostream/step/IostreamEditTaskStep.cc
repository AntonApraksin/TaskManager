#include "IostreamStep.h"
#include "repl/view/steps/ISmallStepFactory.h"
#include "repl/view/steps/Strings.h"
#include "repl/view/steps/TaskContext.h"
#include "repl/view/steps/TaskInitializerSmallStep.h"

namespace task_manager {
IostreamEditTaskStep::IostreamEditTaskStep(
    const std::shared_ptr<IIoFacility> &io_facility,
    const std::shared_ptr<ISmallStepFactory> &state_factory,
    const std::shared_ptr<IValidator> &validator)
    : IostreamStep(io_facility),
      IostreamWithSmallStepStep(state_factory),
      IostreamWithValidatorStep(validator) {}

StepResult IostreamEditTaskStep::Run() {
  std::stringstream ss;
  io_facility_->Print(Strings::kYouAreGoingToEdit);
  io_facility_->Print(Strings::ShowSolidTask(solid_task_));
  TaskContext sub_context;
  sub_context.PushState(std::make_shared<DefaultTaskInitializerSmallStep>(
      TaskBuilder{/*.title =*/solid_task_.task().title(),
                  /*.date_ =*/solid_task_.task().due_date(),
                  /*.priority =*/solid_task_.task().priority(),
                  /*.progress =*/solid_task_.task().progress()}));
  sub_context.PushState(
      small_step_factory_->GetREPLState(IostreamSmallStepEnum::kReadTitle));
  sub_context.PushState(
      small_step_factory_->GetREPLState(IostreamSmallStepEnum::kReadDate));
  sub_context.PushState(
      small_step_factory_->GetREPLState(IostreamSmallStepEnum::kReadPriority));
  sub_context.PushState(
      small_step_factory_->GetREPLState(IostreamSmallStepEnum::kReadState));
  sub_context.Run();

  io_facility_->Print(Strings::ProceedTo("edit"));

  std::string input = io_facility_->GetLine();
  auto confirm = validator_->ParseConfirmation(input);
  if (!confirm) {
    io_facility_->Print(Strings::kOkayITreatItAsNo);
    return StepResult{ConfirmationResult::kNo,
                      sub_context.GetTaskBuilder().GetTask()};
  }
  return {*confirm, sub_context.GetTaskBuilder().GetTask()};
}
}  // namespace task_manager