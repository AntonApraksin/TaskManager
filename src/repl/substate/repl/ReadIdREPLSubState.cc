#include "IREPLSubState.h"
#include "repl/substate/TaskContext.h"

void ReadIdREPLSubState::Execute(TaskContext &ctx) {
  auto id = validator_->ParseTaskId(printer_->AskForAnId());
  if (!id) {
    printer_->ReportNotValidId();
    return;
  }
  ctx.GetTaskId() = *id;
  ctx.PopState();
}
