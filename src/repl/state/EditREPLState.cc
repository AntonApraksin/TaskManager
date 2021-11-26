#include "repl/context/Context.h"
#include "repl/state/IREPLState.h"
#include "repl/substate/SubContext.h"

std::shared_ptr<IREPLState> EditREPLState::Execute(Context &ctx) {
  printer_->ChangePrompt("edit");

  SubContext sub_context;
  sub_context.PushState(ctx.GetStateFactory().GetSubState(SubStateEnum::kReadId));
  sub_context.PushState(ctx.GetStateFactory().GetSubState(SubStateEnum::kReadConfirmation));
  sub_context.PushState(ctx.GetStateFactory().GetSubState(SubStateEnum::kReadTitle));
  sub_context.PushState(ctx.GetStateFactory().GetSubState(SubStateEnum::kReadDate));
  sub_context.PushState(ctx.GetStateFactory().GetSubState(SubStateEnum::kReadPriority));
  sub_context.PushState(ctx.GetStateFactory().GetSubState(SubStateEnum::kReadConfirmation));
  sub_context.PushState(ctx.GetStateFactory().GetCommitState(CommitStateEnum::kEdit));
  sub_context.Run();

  return ctx.GetStateFactory().GetState(StateEnum::kMain);
}
