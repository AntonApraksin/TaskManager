#include "IostreamSmallStep.h"
#include "repl/view/small_step/TaskContext.h"

void IostreamReadPrioritySmallStep::Execute(TaskContext &ctx) {
  auto priority = validator_->ParseTaskPriority(printer_->AskForAPriority());
  for (; !priority;) {
    printer_->ReportNotValidPriority();
    priority = validator_->ParseTaskPriority(printer_->AskForAPriority());
  }
  ctx.GetTaskBuilder().SetPriority(*priority);
  ctx.PopState();
}