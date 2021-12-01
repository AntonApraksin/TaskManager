#include "IREPLSubState.h"
#include "repl/substate/TaskContext.h"

void ReadDateREPLSubState::Execute(TaskContext &ctx) {
  auto date = validator_->ParseTaskDate(printer_->AskForADate());
  for (; !date;) {
    printer_->ReportNotValidDate();
    date = validator_->ParseTaskDate(printer_->AskForADate());
  }
  ctx.GetTaskBuilder().SetDate(*date);
  ctx.PopState();
}