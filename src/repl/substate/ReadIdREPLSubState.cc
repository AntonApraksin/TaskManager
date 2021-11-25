#include "repl/substate/IREPLSubState.h"

std::shared_ptr<IREPLSubState> ReadDateREPLSubState::Execute(Context &) {
  auto id = validator_->ParseTaskId(printer_->AskForAnId());
  for (; !id;) {
    id = validator_->ParseTaskId(printer_->AskForAnId());
  }
  // TODO: forward id somewhere
  return {}; // TODO: put in factory
}
