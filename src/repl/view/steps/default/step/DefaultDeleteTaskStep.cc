#include "DefaultStep.h"
#include "repl/view/steps/Strings.h"

namespace task_manager {
DefaultDeleteTaskStep::DefaultDeleteTaskStep(
    const std::shared_ptr<IIoFacility>& io_facility,
    const std::shared_ptr<IValidator>& validator)
    : DefaultStep(io_facility), DefaultWithValidatorStep(validator) {}

StepResult DefaultDeleteTaskStep::Run() {
  io_facility_->Print(Strings::YouAreGoingTo("delete"));
  io_facility_->Print(Strings::ShowSolidTasks(solid_tasks_));

  io_facility_->Print(Strings::ProceedTo("delete"));
  std::string input = io_facility_->GetLine();
  auto confirm = validator_->ParseConfirmation(input);
  if (!confirm) {
    io_facility_->Print(Strings::kOkayITreatItAsNo);
    return StepResult{ConfirmationResult::kNo, {}};
  }
  return {*confirm, {}};
}
}  // namespace task_manager
