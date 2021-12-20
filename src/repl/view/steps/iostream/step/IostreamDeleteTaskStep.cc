#include "IostreamStep.h"
#include "repl/view/steps/Strings.h"

IostreamDeleteTaskStep::IostreamDeleteTaskStep(
    const std::shared_ptr<IIoFacility>& io_facility,
    const std::shared_ptr<IValidator>& validator)
    : IostreamStep(io_facility), IostreamWithValidatorStep(validator) {}

StepResult IostreamDeleteTaskStep::Run() {
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
