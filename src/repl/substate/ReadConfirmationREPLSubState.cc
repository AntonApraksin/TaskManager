#include "repl/context/Context.h"
#include "repl/substate/IREPLSubState.h"

std::shared_ptr<IREPLSubState> ReadConfirmationREPLSubState::Execute(
    Context &ctx) {
  auto confirm = validator_->ParseConfirmation(printer_->AskForAConfirmation());
  for (; !confirm;) {
    printer_->ReportNotValidConfirmation();
    confirm = validator_->ParseConfirmation(printer_->AskForAConfirmation());
  }
  switch (*confirm) {
    case ConfirmationResult::kYes:
      break;
    case ConfirmationResult::kNo:
      ctx.SetCommitState(
          ctx.GetStateFactory().GetCommitState(CommitStateEnum::kNothing));
  }
  return nullptr;  // TODO: wrap somewhere
}