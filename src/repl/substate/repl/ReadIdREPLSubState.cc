#include "IREPLSubState.h"
#include "repl/substate/SubContext.h"

void ReadIdREPLSubState::Execute(SubContext &ctx) {
  auto id = validator_->ParseTaskId(printer_->AskForAnId());
  if (!id) {
    printer_->ReportNotValidId();
    return;
  }
  ctx.GetTaskId() = *id;
  ctx.PopState();
}
