#include "repl/context/Context.h"
#include "repl/state/IREPLState.h"
#include "repl/substate/SubContext.h"

std::shared_ptr<IREPLState> AddNewREPLState::Execute(Context& ctx) {
  printer_->ChangePrompt("add");
  SubContext sub_context;
  sub_context.PushState(ctx.GetStateFactory().GetSubState(SubStateEnum::kReadTitle));
  sub_context.PushState(ctx.GetStateFactory().GetSubState(SubStateEnum::kReadDate));
  sub_context.PushState(ctx.GetStateFactory().GetSubState(SubStateEnum::kReadPriority));
  sub_context.PushState(ctx.GetStateFactory().GetSubState(SubStateEnum::kReadConfirmation));
  sub_context.PushState(ctx.GetStateFactory().GetCommitState(CommitStateEnum::kAdd));
  sub_context.Run();
  return ctx.GetStateFactory().GetState(StateEnum::kMain);
}