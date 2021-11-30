#include "IREPLSubState.h"
#include "repl/substate/TaskContext.h"

void ReadDateREPLSubState::Execute(TaskContext &ctx) {
  auto date = validator_->ParseTaskDate(printer_->AskForADate());
  if (!date) {
    printer_->ReportNotValidDate();
    return;
  }
  ctx.GetTaskBuilder().SetDate(*date);
  ctx.PopState();
}