#include "IostreamSmallStep.h"
#include "repl/view/small_step/TaskContext.h"

void IostreamReadDateSmallStep::Execute(TaskContext &ctx) {
  auto date = validator_->ParseTaskDate(printer_->AskForADate());
  for (; !date;) {
    printer_->ReportNotValidDate();
    date = validator_->ParseTaskDate(printer_->AskForADate());
  }
  ctx.GetTaskBuilder().SetDate(*date);
  ctx.PopState();
}