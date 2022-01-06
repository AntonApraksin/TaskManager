#include "repl/state_machine/commands/Commands.h"

namespace task_manager {
DeleteTasksCommand::DeleteTasksCommand(std::vector<TaskId> task_ids)
    : task_ids_(std::move(task_ids)) {}

Context DeleteTasksCommand::execute(ModelController& model_controller) {
  Context ctx;
  std::for_each(
      task_ids_.cbegin(), task_ids_.cend(),
      [&model_controller](auto id) { auto _ = model_controller.Delete(id); });
  ctx.status = ModelController::Status::kOk;
  return ctx;
}
}  // namespace task_manager
