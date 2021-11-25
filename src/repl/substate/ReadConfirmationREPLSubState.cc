#include "repl/substate/IREPLSubState.h"

std::shared_ptr<IREPLSubState> ReadConfirmationREPLSubState::Execute(
    Context &) {
  auto confirm = validator_->ParseConfirmation(printer_->AskForAConfirmation());
  for (; !confirm;) {
    confirm = validator_->ParseConfirmation(printer_->AskForAConfirmation());
  }
  // TODO: do something with confirmation
  return nullptr;  // TODO: put in factory
}