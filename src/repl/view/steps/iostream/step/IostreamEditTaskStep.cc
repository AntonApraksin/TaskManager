#include <iostream>

#include "IostreamStep.h"
#include "repl/view/steps/ISmallStepFactory.h"
#include "repl/view/steps/TaskContext.h"
#include "repl/view/steps/TaskInitializerSmallStep.h"
#include "repl/view/steps/iostream/IostreamGeneralFunctional.h"

IostreamEditTaskStep::IostreamEditTaskStep(
    TaskWrapperRef task_wrapper,
    const std::shared_ptr<ISmallStepFactory> &state_factory,
    const std::shared_ptr<IValidator> &validator)
    : IEditTaskStep(task_wrapper),
      IostreamStep(state_factory),
      IostreamWithValidatorStep(validator) {}

StepResult IostreamEditTaskStep::Run() {
  std::cout << "You are going to edit:\n";
  const auto &to_edit = *(task_wrapper_.get());
  ShowTask(to_edit);
  std::cout << "  it has " << task_wrapper_.get().ShowStorage().size()
            << " children.\n";
  TaskContext sub_context;
  sub_context.PushState(std::make_shared<DefaultTaskInitializerSmallStep>(
      TaskBuilder{/*.title =*/to_edit.GetTitle(),
                  /*.date_ =*/to_edit.GetDueDate(),
                  /*.priority =*/to_edit.GetPriority(),
                  /*.state =*/to_edit.GetState()}));
  sub_context.PushState(
      state_factory_->GetREPLState(IostreamSmallStepEnum::kReadTitle));
  sub_context.PushState(
      state_factory_->GetREPLState(IostreamSmallStepEnum::kReadDate));
  sub_context.PushState(
      state_factory_->GetREPLState(IostreamSmallStepEnum::kReadPriority));
  sub_context.PushState(
      state_factory_->GetREPLState(IostreamSmallStepEnum::kReadState));
  sub_context.Run();

  std::cout << "Proceed to edit? [Y/n]: ";

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