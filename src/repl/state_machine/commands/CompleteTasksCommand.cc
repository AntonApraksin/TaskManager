#include <algorithm>

#include "repl/state_machine/commands/Commands.h"

namespace task_manager {
CompleteTasksCommand::CompleteTasksCommand(std::vector<TaskId> task_ids)
    : task_ids_(std::move(task_ids)) {}

Context CompleteTasksCommand::execute(ModelController& model_controller) {
  Context ctx;
  ctx.status = ModelController::Status::kOk;
  std::for_each(task_ids_.cbegin(), task_ids_.cend(),
                [&model_controller, &ctx](auto id) {
                  auto status = model_controller.Complete(id).GetStatus();
                  if (status != ModelController::Status::kOk) {
                    ctx.status = status;
                  }
                });
  return ctx;
}
}  // namespace task_manager
