#include "ICommitState.h"
#include "repl/substate/TaskContext.h"

void AddCommitState::Execute(TaskContext& ctx) {
#if 0
  task_manager_->Add(ctx.GetTaskBuilder().GetTask());
#endif
  ctx.PopState();
}