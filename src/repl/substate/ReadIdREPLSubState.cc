#include "repl/context/Context.h"
#include "repl/substate/IREPLSubState.h"

std::shared_ptr<IREPLSubState> ReadIdREPLSubState::Execute(Context &ctx) {
  auto id = validator_->ParseTaskId(printer_->AskForAnId());
  for (; !id;) {
    printer_->ReportNotValidId();
    id = validator_->ParseTaskId(printer_->AskForAnId());
  }

  ctx.GetTaskId() = *id;
  return nullptr;  // TODO: wrap somewhere
}
