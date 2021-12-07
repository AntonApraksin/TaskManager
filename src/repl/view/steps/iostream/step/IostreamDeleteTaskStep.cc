#include <sstream>

#include "IostreamStep.h"
#include "repl/view/steps/iostream/IostreamGeneralFunctional.h"

IostreamDeleteTaskStep::IostreamDeleteTaskStep(
    const std::shared_ptr<IIoFacility>& io_facility,
    const std::shared_ptr<IValidator>& validator, TaskWrappers task_wrappers)
    : IDeleteTaskStep(std::move(task_wrappers)),
      IostreamStep(io_facility),
      IostreamWithValidatorStep(validator) {}

StepResult IostreamDeleteTaskStep::Run() {
  std::stringstream ss;
  ss << "You are going to delete such tasks:\n";
  io_facility_->Print(ss.str());
  ss.str("");
  for (const auto i : task_wrappers_) {
    ShowTask(*io_facility_, *(i.get()));
    ss << "  and its " << i.get().ShowStorage().size() << " children.\n";
    io_facility_->Print(ss.str());
    ss.str("");
  }

  ss << "Proceed to delete? [Y/n]: ";
  io_facility_->Print(ss.str());
  ss.str("");
  std::string input = io_facility_->GetLine();
  auto confirm = validator_->ParseConfirmation(input);
  if (!confirm) {
    ss << "Okay, I treat it as no\n";
    io_facility_->Print(ss.str());
    ss.str("");
    return StepResult{ConfirmationResult::kNo, {}};
  }
  return {*confirm, {}};
}
