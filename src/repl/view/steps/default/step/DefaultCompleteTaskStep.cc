#include "DefaultStep.h"
#include "repl/view/steps/Strings.h"

namespace task_manager {
DefaultCompleteTaskStep::DefaultCompleteTaskStep(
    const std::shared_ptr<IIoFacility>& io_facility,
    const std::shared_ptr<IValidator>& validator)
    : DefaultStep(io_facility), DefaultWithValidatorStep(validator) {}

StepResult DefaultCompleteTaskStep::Run() {
  io_facility_->Print(Strings::YouAreGoingTo("complete"));

  io_facility_->Print(Strings::ShowSolidTasks(solid_tasks_));

  io_facility_->Print(Strings::ProceedTo("complete"));
  std::string input = io_facility_->GetLine();
  auto confirm = validator_->ParseConfirmation(input);
  if (!confirm) {
    io_facility_->Print(Strings::kOkayITreatItAsNo);
    return StepResult{ConfirmationResult::kNo, {}};
  }
  return {*confirm, {}};
}
}  // namespace task_manager