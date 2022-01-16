#include "interactor/state_machine/commands/Commands.h"

namespace task_manager {
Context VoidCommand::execute(ModelController &) {
  Context ctx;
  ctx.status = ModelController::Status::kOk;
  return ctx;
}
}  // namespace task_manager
