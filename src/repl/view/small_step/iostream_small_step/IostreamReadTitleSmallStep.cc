#include "IostreamSmallStep.h"
#include "repl/view/small_step/TaskContext.h"

void IostreamReadTitleSmallStep::Execute(TaskContext &ctx) {
  auto title = validator_->ValidateTitle(printer_->AskForATitle());
  for (; !title;) {
    printer_->ReportNotValidTitle();
    title = validator_->ValidateTitle(printer_->AskForATitle());
  }
  ctx.GetTaskBuilder().SetTitle(*title);
  ctx.PopState();
}