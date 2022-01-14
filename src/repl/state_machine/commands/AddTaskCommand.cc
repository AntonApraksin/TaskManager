#include "repl/state_machine/commands/Commands.h"

namespace task_manager {
AddTaskCommand::AddTaskCommand(Task task) : task_(std::move(task)) {}

Context AddTaskCommand::execute(ModelController& model_controller) {
  Context ctx;
  auto result = model_controller.Add(task_);
  if (result) {
    ctx.task_id = result.AccessResult();
  }
  ctx.status = result.GetStatus();
  return ctx;
}
}  // namespace task_manager
