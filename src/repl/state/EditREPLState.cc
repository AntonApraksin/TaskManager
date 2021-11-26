#include "repl/context/Context.h"
#include "repl/state/IREPLState.h"
#include "repl/substate/IREPLSubState.h"

std::shared_ptr<IREPLState> EditREPLState::Execute(Context &ctx) {
  printer_->ChangePrompt("edit");

  std::shared_ptr<IREPLSubState> sub_state =
      ctx.GetStateFactory().GetSubState(SubStateEnum::kReadId);
  auto commit_state =
      ctx.GetStateFactory().GetCommitState(CommitStateEnum::kEdit);
  ctx.SetSubState(sub_state);
  ctx.SetCommitState(commit_state);
  ctx.RunInteractor();
  sub_state = ctx.GetStateFactory().GetSubState(SubStateEnum::kReadTitle);
  ctx.SetSubState(sub_state);
  ctx.RunInteractor();
  ctx.Commit();
  return ctx.GetStateFactory().GetState(StateEnum::kMain);
}
