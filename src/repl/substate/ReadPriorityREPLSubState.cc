#include "repl/substate/IREPLSubState.h"

std::shared_ptr<IREPLSubState> ReadPriorityREPLSubState::Execute(Context &) {
  auto priority = validator_->ParseTaskPriority(printer_->AskForAPriority());
  for(; !priority;)
  {
    priority = validator_->ParseTaskPriority(printer_->AskForAPriority());
  }
  // TODO: forward priority somewhere
  return std::make_shared<ReadConfirmationREPLSubState>(printer_, validator_); // TODO: put in factory
}