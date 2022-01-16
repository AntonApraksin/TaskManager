#include "interactor/state_machine/commands/Commands.h"

namespace task_manager {
SaveTasksToFileCommand::SaveTasksToFileCommand(std::string filename)
    : persistence_(std::move(filename)) {}

Context SaveTasksToFileCommand::execute(ModelController& model_controller) {
  Context ctx;
  auto result = model_controller.SaveTo(persistence_);
  ctx.status = result.GetStatus();
  return ctx;
}
}  // namespace task_manager
