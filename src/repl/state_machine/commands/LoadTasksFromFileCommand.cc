#include "repl/state_machine/commands/Commands.h"

namespace task_manager {
LoadTasksFromFileCommand::LoadTasksFromFileCommand(std::string filename)
    : persistence_(std::move(filename)) {}

Context LoadTasksFromFileCommand::execute(ModelController& model_controller) {
  Context ctx;
  auto result = model_controller.LoadFrom(persistence_);
  ctx.status = result.GetStatus();
  return ctx;
}
}  // namespace task_manager
