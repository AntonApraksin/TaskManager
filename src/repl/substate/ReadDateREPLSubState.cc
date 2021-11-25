#include "repl/substate/IREPLSubState.h"

std::shared_ptr<IREPLSubState> ReadDateREPLSubState::Execute(Context &) {
  auto date = validator_->ParseTaskDate(printer_->AskForADate());
  for (; !date;) {
    date = validator_->ParseTaskDate(printer_->AskForADate());
  }
  // TODO: forward date somewhere
  return std::make_shared<ReadPriorityREPLSubState>(
      printer_, validator_);  // TODO: put in factory
}