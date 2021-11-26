#include "repl/context/Context.h"
#include "repl/state/IREPLState.h"

std::shared_ptr<IREPLState> MainREPLState::Execute(Context &ctx) {
  printer_->ChangePrompt("");
  auto action = printer_->AskForAnAction();
  return ctx.GetStateFactory().GetState(validator_->MatchState(action));
}