#include "repl/context/Context.h"
#include "repl/state/IREPLState.h"
#include "repl/substate/TaskContext.h"

std::shared_ptr<IREPLState> EditREPLState::Execute(Context &ctx) {
  printer_->ChangePrompt("edit");

  TaskContext sub_context;
  sub_context.PushState(ctx.GetStateFactory().GetREPLState(SubStateEnum::kReadId));
  sub_context.PushState(ctx.GetStateFactory().GetREPLState(SubStateEnum::kReadConfirmation));
  sub_context.PushState(ctx.GetStateFactory().GetREPLState(SubStateEnum::kReadTitle));
  sub_context.PushState(ctx.GetStateFactory().GetREPLState(SubStateEnum::kReadDate));
  sub_context.PushState(ctx.GetStateFactory().GetREPLState(SubStateEnum::kReadPriority));
  sub_context.PushState(ctx.GetStateFactory().GetREPLState(SubStateEnum::kReadConfirmation));
  sub_context.PushState(ctx.GetStateFactory().GetCommitState(CommitStateEnum::kEdit));
  sub_context.Run();

  return ctx.GetStateFactory().GetState(StateEnum::kMain);
}
