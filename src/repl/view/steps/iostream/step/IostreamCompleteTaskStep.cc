#include <iostream>

#include "IostreamStep.h"
#include "repl/view/steps/iostream/IostreamGeneralFunctional.h"

IostreamCompleteTaskStep::IostreamCompleteTaskStep(
    const std::shared_ptr<IIoFacility>& io_facility,
    const std::shared_ptr<IValidator>& validator, TaskWrappers task_wrappers)
    : ICompleteTaskStep(std::move(task_wrappers)),
      IostreamStep(io_facility),
      IostreamWithValidatorStep(validator) {}

StepResult IostreamCompleteTaskStep::Run() {
  std::cout << "You are going to complete such tasks:\n";
  for (const auto i : task_wrappers_) {
    ShowTask(*(i.get()));
    std::cout << "  and its " << i.get().ShowStorage().size() << " children.\n";
  }

  std::cout << "Proceed to complete? [Y/n]: ";
  std::string input;
  std::getline(std::cin, input);
  auto confirm = validator_->ParseConfirmation(input);
  if (!confirm) {
    std::cout << "Okay, I treat it as no\n";
    return StepResult{ConfirmationResult::kNo, {}};
  }
  return {*confirm, {}};
}
