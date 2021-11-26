#include "repl/context/Context.h"
#include "repl/state/IREPLState.h"
#include "repl/substate/IREPLSubState.h"

std::shared_ptr<IREPLState> DeleteREPLState::Execute(Context &ctx) {
  printer_->ChangePrompt("delete");

  auto sub_state = ctx.GetStateFactory().GetSubState(SubStateEnum::kReadId);
  auto commit_state =
      ctx.GetStateFactory().GetCommitState(CommitStateEnum::kDelete);
  ctx.SetSubState(sub_state);
  ctx.SetCommitState(commit_state);
  ctx.RunInteractor();
  ctx.Commit();
  return ctx.GetStateFactory().GetState(StateEnum::kMain);
}
