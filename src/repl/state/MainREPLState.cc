#include "MainREPLState.h"

#include "repl/context/Context.h"

MainREPLState::MainREPLState(const std::shared_ptr<IPrinter> &printer,
                             const std::shared_ptr<IValidator> &validator)
    : printer_(printer), validator_(validator) {}

std::shared_ptr<IREPLState> MainREPLState::Execute(Context &ctx) {
  printer_->ChangePrompt("");
  auto action = printer_->AskForAnAction();
  return ctx.GetStateFactory().GetState(validator_->MatchState(action));
}