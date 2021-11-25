#include "repl/context/Context.h"
#include "repl/state/IREPLState.h"

StateEnum UnknownREPLState::Execute(const std::shared_ptr<Context>&) {
  printer_->ReportUnknownCommand();
  return StateEnum::kMain;
}
