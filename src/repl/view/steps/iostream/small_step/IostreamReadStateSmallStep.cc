#include "IostreamSmallStep.h"
#include "repl/view/steps/TaskContext.h"
#include "repl/view/steps/iostream/IostreamStrings.h"

// TODO: Prettify implementation

void IostreamReadStateSmallStep::Execute(TaskContext &ctx) {
  if (ctx.GetTaskBuilder().state_) {
    io_facility_->Print(IostreamStrings::kInvalidState);
    io_facility_->Print(IostreamStrings::LeaveEmptyFor(
        IostreamStrings::to_string(*ctx.GetTaskBuilder().state_)));
  }
  std::string prompt = IostreamStrings::GetPrompt("state");
  auto state_string = PrintAndGet(*io_facility_, prompt);
  if (state_string.empty()) {
    ctx.PopState();
    return;
  }
  auto validated_state = validator_->ParseTaskState(state_string);
  for (; !validated_state;) {
    io_facility_->Print(IostreamStrings::kInvalidState);
    state_string = PrintAndGet(*io_facility_, prompt);
    if (state_string.empty()) {
      ctx.PopState();
      return;
    }
    validated_state = validator_->ParseTaskState(state_string);
  }
  ctx.GetTaskBuilder().state_ = validated_state;
  ctx.PopState();
}
