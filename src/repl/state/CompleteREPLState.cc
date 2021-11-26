#include "repl/context/Context.h"
#include "repl/state/IREPLState.h"
#include "repl/substate/SubContext.h"

std::shared_ptr<IREPLState> CompleteREPLState::Execute(Context &ctx) {
  printer_->ChangePrompt("complete");

  SubContext sub_context;
  sub_context.PushState(ctx.GetStateFactory().GetSubState(SubStateEnum::kReadId));
  sub_context.PushState(ctx.GetStateFactory().GetSubState(SubStateEnum::kReadConfirmation));
  sub_context.PushState(ctx.GetStateFactory().GetCommitState(CommitStateEnum::kComplete));
  sub_context.Run();

  return ctx.GetStateFactory().GetState(StateEnum::kMain);
}