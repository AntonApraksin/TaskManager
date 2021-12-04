#include <iostream>

#include "IostreamStep.h"
#include "repl/view/steps/ISmallStepFactory.h"
#include "repl/view/steps/TaskContext.h"
#include "repl/view/steps/TaskInitializerSmallStep.h"
#include "repl/view/steps/iostream/IostreamGeneralFunctional.h"

IostreamAddTaskREPLState::IostreamAddTaskREPLState(
    const std::shared_ptr<ISmallStepFactory>& state_factory,
    const std::shared_ptr<IValidator>& validator)
    : IostreamREPLState(state_factory),
      IostreamWithValidatorREPLState(validator) {}

StepResult IostreamAddTaskREPLState::Run() {
  TaskContext sub_context;
  if (task_) {
    std::cout << "Add subtask to:\n";
    ShowTask(*task_);
    sub_context.PushState(std::make_shared<DefaultTaskInitializerSmallStep>(
        TaskBuilder{/*.title = */ std::nullopt,
                    /*.date_ =*/task_->GetDueDate(),
                    /*.priority =*/task_->GetPriority(),
                    /*.state =*/task_->GetState()}));
    task_.reset();
  } else {
    sub_context.PushState(
        std::make_shared<DefaultTaskInitializerSmallStep>(TaskBuilder{
            /*.title =*/std::nullopt,
            /*.date_ =*/std::chrono::system_clock::now(),  // TODO: other
                                                           // default value
            /*.priority =*/Task::Priority::kLow,
            /*.state =*/Task::State::kUncompleted}));
  }

  sub_context.PushState(
      state_factory_->GetREPLState(IostreamSmallStepEnum::kReadTitle));
  sub_context.PushState(
      state_factory_->GetREPLState(IostreamSmallStepEnum::kReadDate));
  sub_context.PushState(
      state_factory_->GetREPLState(IostreamSmallStepEnum::kReadPriority));
  sub_context.PushState(
      state_factory_->GetREPLState(IostreamSmallStepEnum::kReadState));
  sub_context.Run();

  std::cout << "Proceed to add? [Y/n]: ";
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