#include "repl/context/Context.h"
#include "repl/state/IREPLState.h"
#include "repl/substate/IREPLSubState.h"

std::shared_ptr<IREPLState> CompleteREPLState::Execute(Context &ctx) {
  printer_->ChangePrompt("complete");

  auto sub_state = ctx.GetStateFactory().GetSubState(SubStateEnum::kReadId);
  auto commit_state =
      ctx.GetStateFactory().GetCommitState(CommitStateEnum::kComplete);
  ctx.SetSubState(sub_state);
  ctx.RunInteractor();
  ctx.Commit();
  return ctx.GetStateFactory().GetState(StateEnum::kMain);
}