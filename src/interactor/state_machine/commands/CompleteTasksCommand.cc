#include <algorithm>

#include "interactor/state_machine/commands/Commands.h"

namespace task_manager {
CompleteTasksCommand::CompleteTasksCommand(std::vector<TaskId> task_ids)
    : task_ids_(std::move(task_ids)) {}

CommandResult CompleteTasksCommand::execute(ModelController& model_controller) {
  CommandResult command_result;
  command_result.status = ModelController::Status::kOk;
  std::for_each(task_ids_.cbegin(), task_ids_.cend(),
                [&model_controller, &command_result](auto id) {
                  auto status = model_controller.Complete(id).GetStatus();
                  if (status != ModelController::Status::kOk) {
                    command_result.status = status;
                  }
                });
  return command_result;
}
}  // namespace task_manager
