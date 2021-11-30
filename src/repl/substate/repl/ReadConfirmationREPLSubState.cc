#include "IREPLSubState.h"
#include "repl/substate/TaskContext.h"

void ReadConfirmationREPLSubState::Execute(
    TaskContext &ctx) {
  auto confirm = validator_->ParseConfirmation(printer_->AskForAConfirmation());
  if(!confirm) {
    printer_->ReportNotValidConfirmation();
    return;
  }
  switch (*confirm) {
    case ConfirmationResult::kYes:
      ctx.PopState();
      break;
    case ConfirmationResult::kNo:
      ctx.ClearStates();
      return;
  }
}