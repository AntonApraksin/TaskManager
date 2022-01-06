#include "repl/state_machine/commands/Commands.h"

namespace task_manager {
LoadTasksCommand::LoadTasksCommand(std::unique_ptr<std::istream> istream)
    : istream_(std::move(istream)) {}
Context LoadTasksCommand::execute(ModelController& model_controller) {
  Context ctx;
  auto result = model_controller.LoadFrom(*istream_);
  ctx.status = result.GetStatus();
  return ctx;
}
}  // namespace task_manager
