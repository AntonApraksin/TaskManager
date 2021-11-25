#include "repl/context/Context.h"
#include "repl/state/IREPLState.h"

StateEnum AddNewREPLState::Execute(const std::shared_ptr<Context>&) {
  printer_->ChangePrompt("add");

  auto title = printer_->AskForATitle();
  // validate
  auto date = printer_->AskForADate();
  // validate
  auto priority = printer_->AskForAPriority();
  // validate
  auto confirm = printer_->AskForAConfirmation();
  // if confirm = y then else ...

  return StateEnum::kMain;
}