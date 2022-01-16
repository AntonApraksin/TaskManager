#include "interactor/state_machine/commands/Commands.h"

namespace task_manager {
GetSpecifiedTasksCommand::GetSpecifiedTasksCommand(std::vector<TaskId> task_ids)
    : task_ids_(std::move(task_ids)) {}

Context GetSpecifiedTasksCommand::execute(ModelController &model_controller) {
  Context ctx;
  auto result = model_controller.GetSpecificSolidTasks(std::move(task_ids_));
  if (result) {
    ctx.solid_tasks = result.AccessResult();
  }
  ctx.status = result.GetStatus();
  return ctx;
}
}  // namespace task_manager
