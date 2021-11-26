#include "repl/commit/ICommitState.h"
#include "repl/context/Context.h"
#include "repl/state/IREPLState.h"
#include "repl/substate/IREPLSubState.h"

std::shared_ptr<IREPLState> AddNewREPLState::Execute(Context& ctx) {
  printer_->ChangePrompt("add");
  auto sub_state = ctx.GetStateFactory().GetSubState(SubStateEnum::kReadTitle);
  auto commit_state =
      ctx.GetStateFactory().GetCommitState(CommitStateEnum::kAdd);
  ctx.SetSubState(sub_state);
  ctx.SetCommitState(commit_state);
  ctx.RunInteractor();
  ctx.Commit();
  return ctx.GetStateFactory().GetState(StateEnum::kMain);
}