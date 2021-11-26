#include "repl/commit/ICommitState.h"
#include "repl/context/Context.h"

void DeleteCommitState::Execute(Context & /*ctx*/) {
#if 0
  task_manager_->Delete(ctx.GetTaskId());
#endif
}