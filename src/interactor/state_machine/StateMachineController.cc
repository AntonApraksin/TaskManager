#include "StateMachineController.h"

#include "interactor/state_machine/commands/CommandResult.h"

task_manager::StateMachineController::StateMachineController(
    std::shared_ptr<ModelController> model_controller,
    std::unique_ptr<StateMachine> state_machine)
    : model_controller_(std::move(model_controller)),
      state_machine_(std::move(state_machine)) {}

void task_manager::StateMachineController::Run() {
  Context ctx;
  CommandResult command_result;
  auto command = state_machine_->execute(ctx);
  for (; command;) {
    command_result = command->execute(*model_controller_);
    ctx.task_id = std::move(command_result.task_id);
    ctx.status = command_result.status;
    ctx.solid_tasks = std::move(command_result.solid_tasks);
    command = state_machine_->execute(ctx);
  }
}
