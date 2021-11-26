#include "ICommitState.h"
#include "repl/substate/SubContext.h"

void CompleteCommitState::Execute(SubContext& ctx) {
#if 0
  task_manager_->Complete(ctx.GetTaskId());
#endif
  ctx.PopState();
}