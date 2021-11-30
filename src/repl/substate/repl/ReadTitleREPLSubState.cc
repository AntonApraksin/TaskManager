#include "IREPLSubState.h"
#include "repl/substate/TaskContext.h"

void ReadTitleREPLSubState::Execute(TaskContext &ctx) {
  auto title = validator_->ValidateTitle(printer_->AskForATitle());
  for (; !title;) {
    printer_->ReportNotValidTitle();
    return;
  }
  ctx.GetTaskBuilder().SetTitle(*title);
  ctx.PopState();
}