#include "ICommitState.h"
#include "repl/substate/SubContext.h"

void NothingCommitState::Execute(SubContext &ctx) {
  ctx.PopState();
}