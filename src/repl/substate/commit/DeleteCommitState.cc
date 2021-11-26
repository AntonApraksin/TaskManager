#include "ICommitState.h"
#include "repl/substate/SubContext.h"

void DeleteCommitState::Execute(SubContext& ctx) {
#if 0
  task_manager_->Delete(ctx.GetTaskId());
#endif
  ctx.PopState();
}