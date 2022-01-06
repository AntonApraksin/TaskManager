#include "repl/state_machine/commands/Commands.h"

namespace task_manager {
SaveTasksCommand::SaveTasksCommand(std::unique_ptr<std::ostream> ostream)
    : ostream_(std::move(ostream)) {}
Context SaveTasksCommand::execute(ModelController& model_controller) {
  Context ctx;
  auto result = model_controller.SaveTo(*ostream_);
  ctx.status = result.GetStatus();
  return ctx;
}
}  // namespace task_manager
