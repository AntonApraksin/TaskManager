#include "interactor/state_machine/commands/Commands.h"

namespace task_manager {
AddSubtaskCommand::AddSubtaskCommand(TaskId task_id, Task task)
    : task_id_(std::move(task_id)), task_(std::move(task)) {}

CommandResult AddSubtaskCommand::execute(ModelController& model_controller) {
  CommandResult command_result;
  auto result = model_controller.Add(task_id_, std::move(task_));
  if (result) {
    command_result.task_id = result.AccessResult();
  }
  command_result.status = result.GetStatus();
  return command_result;
}
}  // namespace task_manager
