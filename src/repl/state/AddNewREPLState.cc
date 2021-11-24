#include "AddNewREPLState.h"

#include "repl/context/Context.h"

AddNewREPLState::AddNewREPLState(const std::shared_ptr<IPrinter>& printer,
                                 const std::shared_ptr<IValidator>& validator)
    : printer_(printer), validator_(validator) {}

std::shared_ptr<IREPLState> AddNewREPLState::Execute(Context& ctx) {
  printer_->ChangePrompt("add");

  auto title = printer_->AskForATitle();
  // validate
  auto date = printer_->AskForADate();
  // validate
  auto priority = printer_->AskForAPriority();
  // validate
  auto confirm = printer_->AskForAConfirmation();
  // if confirm = y then else ...

  return ctx.GetStateFactory().GetState(StateEnum::kMain);
}