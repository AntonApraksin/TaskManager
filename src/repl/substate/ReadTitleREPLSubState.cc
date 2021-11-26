#include "repl/context/Context.h"
#include "repl/substate/IREPLSubState.h"

std::shared_ptr<IREPLSubState> ReadTitleREPLSubState::Execute(Context &ctx) {
  auto title = validator_->ValidateTitle(printer_->AskForATitle());
  for (; !title;) {
    printer_->ReportNotValidTitle();
    title = validator_->ValidateTitle(printer_->AskForATitle());
  }
  ctx.GetTaskBuilder().SetTitle(*title);
  return ctx.GetStateFactory().GetSubState(SubStateEnum::kReadDate);
}