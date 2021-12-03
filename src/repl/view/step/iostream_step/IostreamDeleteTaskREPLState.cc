#include <iostream>

#include "repl/view/step/iostream_step/IostreamStep.h"

IostreamDeleteTaskREPLState::IostreamDeleteTaskREPLState(
    TaskWrappers task_wrappers, const std::shared_ptr<IValidator> &validator)
    : IDeleteTaskREPLState(std::move(task_wrappers)),
      IostreamWithValidatorREPLState(validator) {}

StepResult IostreamDeleteTaskREPLState::Run() {
  std::cout << "You are going to delete such tasks:\n";
  for (const auto i : task_wrappers_) {
    std::cout << "  [" << to_string(i.get()->GetState()) << "] "
              << "(" << to_string(i.get()->GetPriority()) << ") "
              << i.get()->GetTitle()
              << "\n";  // TODO: Display date, add children
  }

  std::string input;
  std::getline(std::cin, input);
  auto confirm = validator_->ParseConfirmation(input);
  if (!confirm) {
    std::cout << "Okay, I treat it as no\n";
    return StepResult{ConfirmationResult::kNo, {}};
  }
  return {*confirm, {}};
}
