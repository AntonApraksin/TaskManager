#include "ICommitState.h"
#include "repl/substate/TaskContext.h"

void DeleteCommitState::Execute(TaskContext& ctx) {
#if 0
  task_manager_->Delete(ctx.GetTaskId());
#endif
  ctx.PopState();
}