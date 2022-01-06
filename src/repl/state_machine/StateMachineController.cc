#include "StateMachineController.h"

task_manager::StateMachineController::StateMachineController(
    std::shared_ptr<ModelController> model_controller,
    std::unique_ptr<StateMachine> state_machine)
    : model_controller_(std::move(model_controller)),
      state_machine_(std::move(state_machine)) {}

void task_manager::StateMachineController::Run() {
  Context ctx;
  auto command = state_machine_->execute(ctx);
  for (; command;) {
    ctx = command->execute(*model_controller_);
    command = state_machine_->execute(ctx);
  }
}
