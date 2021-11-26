#include "repl/context/Context.h"
#include "repl/state/IREPLState.h"
#include "repl/substate/SubContext.h"

std::shared_ptr<IREPLState> DeleteREPLState::Execute(Context &ctx) {
  printer_->ChangePrompt("delete");

  SubContext sub_context;
  sub_context.PushState(ctx.GetStateFactory().GetSubState(SubStateEnum::kReadId));
  sub_context.PushState(ctx.GetStateFactory().GetSubState(SubStateEnum::kReadConfirmation));
  sub_context.PushState(ctx.GetStateFactory().GetCommitState(CommitStateEnum::kDelete));
  sub_context.Run();

  return ctx.GetStateFactory().GetState(StateEnum::kMain);
}
