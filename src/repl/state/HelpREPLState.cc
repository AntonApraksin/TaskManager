#include "repl/context/Context.h"
#include "repl/state/IREPLState.h"

StateEnum HelpREPLState::Execute(const std::shared_ptr<Context>&) {
  printer_->ShowHelp();
  return StateEnum::kMain;
}
