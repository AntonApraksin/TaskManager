#include "repl/context/Context.h"
#include "repl/substate/IREPLSubState.h"

std::shared_ptr<IREPLSubState> ReadDateREPLSubState::Execute(Context &ctx) {
  auto date = validator_->ParseTaskDate(printer_->AskForADate());
  for (; !date;) {
    printer_->ReportNotValidDate();
    date = validator_->ParseTaskDate(printer_->AskForADate());
  }
  ctx.GetTaskBuilder().SetDate(*date);
  return ctx.GetStateFactory().GetSubState(SubStateEnum::kReadPriority);
}