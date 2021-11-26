#include "IREPLSubState.h"
#include "repl/substate/SubContext.h"

void ReadPriorityREPLSubState::Execute(SubContext &ctx) {
  auto priority = validator_->ParseTaskPriority(printer_->AskForAPriority());
  for (; !priority;) {
    printer_->ReportNotValidPriority();
    return;
  }
  ctx.GetTaskBuilder().SetPriority(*priority);
  ctx.PopState();
}