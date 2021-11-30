#include "ICommitState.h"
#include "repl/substate/TaskContext.h"

void EditCommitState::Execute(TaskContext& ctx) {
#if 0
  task_manager_->Edit(ctx.GetTaskId(), ctx.GetTaskBuilder().GetTask());
#endif
  ctx.PopState();
}