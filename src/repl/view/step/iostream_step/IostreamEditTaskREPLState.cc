#include "repl/view/step/iostream_step/IostreamStep.h"
#include "repl/state_factory/IStateFactory.h"
#include "repl/substate/TaskContext.h"

#include <iostream>

IostreamEditTaskREPLState::IostreamEditTaskREPLState(
    TaskWrapperRef task_wrapper,
    const std::shared_ptr<IStateFactory> &state_factory,
    const std::shared_ptr<IValidator> &validator)
    : IEditTaskREPLState(task_wrapper),
      IostreamREPLState(state_factory),
      IostreamWithValidatorREPLState(validator) {}

StepResult IostreamEditTaskREPLState::Run() {
  std::cout << "You are going to edit:\n"
      << "  [" << to_string(task_wrapper_.get()->GetState()) << "] "
      << "(" << to_string(task_wrapper_.get()->GetPriority()) << ") "
      << task_wrapper_.get()->GetTitle() << "\n";  // TODO: Display date
      // TODO: inform about children
  TaskContext sub_context;
  sub_context.PushState(state_factory_->GetREPLState(SubStateEnum::kReadTitle));
  sub_context.PushState(state_factory_->GetREPLState(SubStateEnum::kReadDate));
  sub_context.PushState(
      state_factory_->GetREPLState(SubStateEnum::kReadPriority));
  sub_context.Run();

  std::cout << "Proceed to edit [Y/n]? ";

  std::string input;
  std::getline(std::cin, input);
  auto confirm = validator_->ParseConfirmation(input);
  if (!confirm) {
    std::cout << "Okay, I treat it as no\n";
    return StepResult{ConfirmationResult::kNo,
                      sub_context.GetTaskBuilder().GetTask()};
  }
  return {*confirm, sub_context.GetTaskBuilder().GetTask()};
}