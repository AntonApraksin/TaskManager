#include "repl/context/Context.h"
#include "repl/state/IREPLState.h"

StateEnum MainREPLState::Execute(const std::shared_ptr<Context>&) {
  printer_->ChangePrompt("");
  auto action = printer_->AskForAnAction();
  return validator_->MatchState(action);
}