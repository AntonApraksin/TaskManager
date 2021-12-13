#include "IostreamStep.h"
#include "repl/view/steps/iostream/IostreamStrings.h"

IostreamDeleteTaskStep::IostreamDeleteTaskStep(
    const std::shared_ptr<IIoFacility>& io_facility,
    const std::shared_ptr<IValidator>& validator, TaskWrappers task_wrappers)
    : IDeleteTaskStep(std::move(task_wrappers)),
      IostreamStep(io_facility),
      IostreamWithValidatorStep(validator) {}

StepResult IostreamDeleteTaskStep::Run() {
  io_facility_->Print(IostreamStrings::YouAreGoingTo("delete"));
  for (const auto i : task_wrappers_) {
    io_facility_->Print(IostreamStrings::ShowTask(*(i.get())));
    io_facility_->Print(
        IostreamStrings::AndItsChildren(i.get().ShowStorage().size()));
  }

  io_facility_->Print(IostreamStrings::ProceedTo("delete"));
  std::string input = io_facility_->GetLine();
  auto confirm = validator_->ParseConfirmation(input);
  if (!confirm) {
    io_facility_->Print(IostreamStrings::kOkayITreatItAsNo);
    return StepResult{ConfirmationResult::kNo, {}};
  }
  return {*confirm, {}};
}
