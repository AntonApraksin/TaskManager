#include "IostreamStep.h"
#include "repl/view/steps/iostream/IostreamStrings.h"

IostreamCompleteTaskStep::IostreamCompleteTaskStep(
    const std::shared_ptr<IIoFacility>& io_facility,
    const std::shared_ptr<IValidator>& validator, TaskWrappers task_wrappers)
    : ICompleteTaskStep(std::move(task_wrappers)),
      IostreamStep(io_facility),
      IostreamWithValidatorStep(validator) {}

StepResult IostreamCompleteTaskStep::Run() {
  io_facility_->Print(IostreamStrings::YouAreGoingTo("complete"));
  for (const auto i : task_wrappers_) {
    io_facility_->Print(IostreamStrings::ShowTask(*(i.get())));
    io_facility_->Print(
        IostreamStrings::AndItsChildren(i.get().ShowStorage().size()));
  }

  io_facility_->Print(IostreamStrings::ProceedTo("complete"));
  std::string input = io_facility_->GetLine();
  auto confirm = validator_->ParseConfirmation(input);
  if (!confirm) {
    io_facility_->Print(IostreamStrings::kOkayITreatItAsNo);
    return StepResult{ConfirmationResult::kNo, {}};
  }
  return {*confirm, {}};
}
