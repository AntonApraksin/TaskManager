#include "IREPLSubState.h"
#include "repl/substate/SubContext.h"

void ReadTitleREPLSubState::Execute(SubContext &ctx) {
  auto title = validator_->ValidateTitle(printer_->AskForATitle());
  for (; !title;) {
    printer_->ReportNotValidTitle();
    return;
  }
  ctx.GetTaskBuilder().SetTitle(*title);
  ctx.PopState();
}