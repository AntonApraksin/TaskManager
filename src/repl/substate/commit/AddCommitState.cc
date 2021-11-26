#include "ICommitState.h"
#include "repl/substate/SubContext.h"

void AddCommitState::Execute(SubContext& ctx) {
#if 0
  task_manager_->Add(ctx.GetTaskBuilder().GetTask());
#endif
  ctx.PopState();
}