#include "interactor/state_machine/commands/Commands.h"

namespace task_manager {
AddTaskCommand::AddTaskCommand(Task task) : task_(std::move(task)) {}

CommandResult AddTaskCommand::execute(ModelController& model_controller) {
  CommandResult command_result;
  auto result = model_controller.Add(task_);
  if (result) {
    command_result.task_id = result.AccessResult();
  }
  command_result.status = result.GetStatus();
  return command_result;
}
}  // namespace task_manager
