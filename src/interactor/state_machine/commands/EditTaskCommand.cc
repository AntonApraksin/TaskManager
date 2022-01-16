#include "interactor/state_machine/commands/Commands.h"

namespace task_manager {
EditTaskCommand::EditTaskCommand(TaskId task_id, Task task)
    : task_id_(std::move(task_id)), task_(std::move(task)) {}

Context EditTaskCommand::execute(ModelController& model_controller) {
  Context ctx;
  auto result = model_controller.Edit(task_id_, std::move(task_));
  ctx.status = result.GetStatus();
  return ctx;
}
}  // namespace task_manager
