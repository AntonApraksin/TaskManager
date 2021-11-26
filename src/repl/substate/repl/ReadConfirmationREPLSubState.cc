#include "IREPLSubState.h"
#include "repl/substate/SubContext.h"

void ReadConfirmationREPLSubState::Execute(
    SubContext &ctx) {
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