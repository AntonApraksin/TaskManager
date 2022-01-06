#include "repl/state_machine/commands/Commands.h"

namespace task_manager {
AddSubtaskCommand::AddSubtaskCommand(TaskId task_id, Task task)
    : task_id_(std::move(task_id)), task_(std::move(task)) {}

Context AddSubtaskCommand::execute(ModelController& model_controller) {
  Context ctx;
  auto result = model_controller.Add(task_id_, std::move(task_));
  if (result) {
    ctx.status = ModelController::Status::kOk;
    ctx.task_id = result.AccessResult();
    return ctx;
  } else {
    ctx.status = result.GetStatus();
  }
  return ctx;
}
}  // namespace task_manager
