#include "repl/substate/IREPLSubState.h"

std::shared_ptr<IREPLSubState> ReadTitleREPLSubState::Execute(Context &) {
  auto title = validator_->ValidateTitle(printer_->AskForATitle());
  for (; !title;) {
    title = validator_->ValidateTitle(printer_->AskForATitle());
  }
  // TODO: forward title somewhere
  return std::make_shared<ReadDateREPLSubState>(
      printer_, validator_);  // TODO: put in factory
}