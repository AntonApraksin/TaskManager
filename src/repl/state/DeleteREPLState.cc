#include "repl/context/Context.h"
#include "repl/state/IREPLState.h"
#include "repl/substate/TaskContext.h"

std::shared_ptr<IREPLState> DeleteREPLState::Execute(Context &ctx) {
  printer_->ChangePrompt("delete");

  TaskContext sub_context;
  sub_context.PushState(ctx.GetStateFactory().GetREPLState(SubStateEnum::kReadId));
  sub_context.PushState(ctx.GetStateFactory().GetREPLState(SubStateEnum::kReadConfirmation));
  sub_context.PushState(ctx.GetStateFactory().GetCommitState(CommitStateEnum::kDelete));
  sub_context.Run();

  return ctx.GetStateFactory().GetState(StateEnum::kMain);
}
