#include "repl/context/Context.h"
#include "repl/state/IREPLState.h"
#include "repl/substate/TaskContext.h"

std::shared_ptr<IREPLState> AddNewREPLState::Execute(Context& ctx) {
  printer_->ChangePrompt("add");
  TaskContext sub_context;
  sub_context.PushState(ctx.GetStateFactory().GetREPLState(SubStateEnum::kReadTitle));
  sub_context.PushState(ctx.GetStateFactory().GetREPLState(SubStateEnum::kReadDate));
  sub_context.PushState(ctx.GetStateFactory().GetREPLState(SubStateEnum::kReadPriority));
  sub_context.PushState(ctx.GetStateFactory().GetREPLState(SubStateEnum::kReadConfirmation));
  sub_context.PushState(ctx.GetStateFactory().GetCommitState(CommitStateEnum::kAdd));
  sub_context.Run();
  return ctx.GetStateFactory().GetState(StateEnum::kMain);
}