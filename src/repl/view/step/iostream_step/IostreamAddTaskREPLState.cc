#include <iostream>

#include "repl/state_factory/IStateFactory.h"
#include "repl/substate/TaskContext.h"
#include "repl/view/step/iostream_step/IostreamStep.h"

IostreamAddTaskREPLState::IostreamAddTaskREPLState(
    const std::shared_ptr<IStateFactory>& state_factory,
    const std::shared_ptr<IValidator>& validator)
    : IostreamREPLState(state_factory),
      IostreamWithValidatorREPLState(validator) {}

StepResult IostreamAddTaskREPLState::Run() {
  if (task_) {
    std::cout << "Add subtask to:\n"
              << "  [" << to_string(task_->GetState()) << "] "
              << "(" << to_string(task_->GetPriority()) << ") "
              << task_->GetTitle() << "\n";  // TODO: Display date
  }
  TaskContext sub_context;
  sub_context.PushState(state_factory_->GetREPLState(SubStateEnum::kReadTitle));
  sub_context.PushState(state_factory_->GetREPLState(SubStateEnum::kReadDate));
  sub_context.PushState(
      state_factory_->GetREPLState(SubStateEnum::kReadPriority));
  sub_context.Run();

  std::cout << "Proceed to add [Y/n]? ";

  std::string input;
  std::getline(std::cin, input);
  auto confirm = validator_->ParseConfirmation(input);
  if (!confirm) {
    std::cout << "Okay, i treat it as no\n";
    return StepResult{ConfirmationResult::kNo,
                      sub_context.GetTaskBuilder().GetTask()};
  }
  return {*confirm, sub_context.GetTaskBuilder().GetTask()};
}