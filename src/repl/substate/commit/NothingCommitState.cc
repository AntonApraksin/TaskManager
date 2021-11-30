#include "ICommitState.h"
#include "repl/substate/TaskContext.h"

void NothingCommitState::Execute(TaskContext &ctx) {
  ctx.PopState();
}