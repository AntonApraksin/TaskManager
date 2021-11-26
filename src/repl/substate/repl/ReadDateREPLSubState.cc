#include "IREPLSubState.h"
#include "repl/substate/SubContext.h"

void ReadDateREPLSubState::Execute(SubContext &ctx) {
  auto date = validator_->ParseTaskDate(printer_->AskForADate());
  if (!date) {
    printer_->ReportNotValidDate();
    return;
  }
  ctx.GetTaskBuilder().SetDate(*date);
  ctx.PopState();
}