#include "HelpREPLState.h"

#include "repl/context/Context.h"

HelpREPLState::HelpREPLState(const std::shared_ptr<IPrinter> &printer)
    : printer_(printer) {}

std::shared_ptr<IREPLState> HelpREPLState::Execute(Context &ctx) {
  printer_->ShowHelp();
  return ctx.GetStateFactory().GetState(StateEnum::kMain);
}
