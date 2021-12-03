#include <iostream>

#include "repl/view/step/iostream_step/IostreamGeneralFunctional.h"
#include "repl/view/step/iostream_step/IostreamStep.h"

IostreamCompleteTaskREPLState::IostreamCompleteTaskREPLState(
    TaskWrappers task_wrappers, const std::shared_ptr<IValidator> &validator)
    : ICompleteTaskREPLState(std::move(task_wrappers)),
      IostreamWithValidatorREPLState(validator) {}

StepResult IostreamCompleteTaskREPLState::Run() {
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
