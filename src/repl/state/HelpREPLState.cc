#include "repl/context/Context.h"
#include "repl/state/IREPLState.h"

std::shared_ptr<IREPLState> HelpREPLState::Execute(Context& ctx) {
  printer_->ShowHelp();
  return ctx.GetStateFactory().GetState(StateEnum::kMain);
}
