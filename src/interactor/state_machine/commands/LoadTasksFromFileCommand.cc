#include "interactor/state_machine/commands/Commands.h"

namespace task_manager {
LoadTasksFromFileCommand::LoadTasksFromFileCommand(std::string filename)
    : persistence_(std::move(filename)) {}

CommandResult LoadTasksFromFileCommand::execute(
    ModelController& model_controller) {
  CommandResult command_result;
  auto result = model_controller.LoadFrom(persistence_);
  command_result.status = result.GetStatus();
  return command_result;
}
}  // namespace task_manager
