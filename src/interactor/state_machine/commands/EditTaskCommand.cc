#include "interactor/state_machine/commands/Commands.h"

namespace task_manager {
EditTaskCommand::EditTaskCommand(TaskId task_id, Task task)
    : task_id_(std::move(task_id)), task_(std::move(task)) {}

CommandResult EditTaskCommand::execute(ModelController& model_controller) {
  CommandResult command_result;
  auto result = model_controller.Edit(task_id_, std::move(task_));
  command_result.status = result.GetStatus();
  return command_result;
}
}  // namespace task_manager
