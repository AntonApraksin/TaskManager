#include "interactor/state_machine/commands/Commands.h"

namespace task_manager {
GetSpecifiedTasksCommand::GetSpecifiedTasksCommand(std::vector<TaskId> task_ids)
    : task_ids_(std::move(task_ids)) {}

CommandResult GetSpecifiedTasksCommand::execute(
    ModelController &model_controller) {
  CommandResult command_result;
  auto result = model_controller.GetSpecificSolidTasks(std::move(task_ids_));
  if (result) {
    command_result.solid_tasks = result.AccessResult();
  }
  command_result.status = result.GetStatus();
  return command_result;
}
}  // namespace task_manager
