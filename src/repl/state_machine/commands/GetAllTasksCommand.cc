#include "repl/state_machine/commands/Commands.h"

namespace task_manager {
Context GetAllTasksCommand::execute(ModelController& model_controller) {
  Context ctx;
  auto result = model_controller.GetAllSolidTasks();
  if (result) {
    ctx.status = ModelController::Status::kOk;
    ctx.solid_tasks = result.AccessResult();
    return ctx;
  }
  ctx.status = result.GetStatus();
  return ctx;
}
}  // namespace task_manager
