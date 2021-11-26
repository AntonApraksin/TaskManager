#include "repl/context/Context.h"
#include "repl/substate/IREPLSubState.h"

std::shared_ptr<IREPLSubState> ReadPriorityREPLSubState::Execute(Context &ctx) {
  auto priority = validator_->ParseTaskPriority(printer_->AskForAPriority());
  for (; !priority;) {
    printer_->ReportNotValidPriority();
    priority = validator_->ParseTaskPriority(printer_->AskForAPriority());
  }
  ctx.GetTaskBuilder().SetPriority(*priority);
  return ctx.GetStateFactory().GetSubState(SubStateEnum::kReadConfirmation);
}