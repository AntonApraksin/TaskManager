#include "IREPLSubState.h"
#include "repl/substate/TaskContext.h"

void ReadPriorityREPLSubState::Execute(TaskContext &ctx) {
  auto priority = validator_->ParseTaskPriority(printer_->AskForAPriority());
  for (; !priority;) {
    printer_->ReportNotValidPriority();
    priority = validator_->ParseTaskPriority(printer_->AskForAPriority());
  }
  ctx.GetTaskBuilder().SetPriority(*priority);
  ctx.PopState();
}