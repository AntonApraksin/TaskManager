#include <iostream>

#include "IostreamStep.h"
#include "repl/view/steps/iostream/IostreamGeneralFunctional.h"

IostreamDeleteTaskStep::IostreamDeleteTaskStep(
    TaskWrappers task_wrappers, const std::shared_ptr<IValidator> &validator)
    : IDeleteTaskStep(std::move(task_wrappers)),
      IostreamWithValidatorStep(validator) {}

StepResult IostreamDeleteTaskStep::Run() {
  std::cout << "You are going to delete such tasks:\n";
  for (const auto i : task_wrappers_) {
    ShowTask(*(i.get()));
    std::cout << "  and its " << i.get().ShowStorage().size() << " children.\n";
  }

  std::cout << "Proceed to delete? [Y/n]: ";
  std::string input;
  std::getline(std::cin, input);
  auto confirm = validator_->ParseConfirmation(input);
  if (!confirm) {
    std::cout << "Okay, I treat it as no\n";
    return StepResult{ConfirmationResult::kNo, {}};
  }
  return {*confirm, {}};
}
