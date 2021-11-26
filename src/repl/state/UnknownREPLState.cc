#include "repl/context/Context.h"
#include "repl/state/IREPLState.h"

std::shared_ptr<IREPLState> UnknownREPLState::Execute(Context &ctx) {
  printer_->ReportUnknownCommand();
  return ctx.GetStateFactory().GetState(StateEnum::kMain);
}
