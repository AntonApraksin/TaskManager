#include "ICommitState.h"
#include "repl/substate/TaskContext.h"

void CompleteCommitState::Execute(TaskContext& ctx) {
#if 0
  task_manager_->Complete(ctx.GetTaskId());
#endif
  ctx.PopState();
}