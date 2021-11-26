#include "repl/commit/ICommitState.h"
#include "repl/context/Context.h"

void EditCommitState::Execute(Context & /*ctx*/) {
#if 0
  task_manager_->Edit(ctx.GetTaskId(), ctx.GetTaskBuilder().GetTask());
#endif
}