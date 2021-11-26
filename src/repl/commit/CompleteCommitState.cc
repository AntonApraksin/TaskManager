#include "repl/commit/ICommitState.h"
#include "repl/context/Context.h"

void CompleteCommitState::Execute(Context & /*ctx*/) {
#if 0
  task_manager_->Complete(ctx.GetTaskId());
#endif
}