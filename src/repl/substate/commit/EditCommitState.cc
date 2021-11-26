#include "ICommitState.h"
#include "repl/substate/SubContext.h"

void EditCommitState::Execute(SubContext& ctx) {
#if 0
  task_manager_->Edit(ctx.GetTaskId(), ctx.GetTaskBuilder().GetTask());
#endif
  ctx.PopState();
}