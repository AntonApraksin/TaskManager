#include "IostreamSmallStep.h"
#include "repl/view/steps/TaskContext.h"
#include "repl/view/steps/iostream/IostreamStrings.h"

// TODO: Prettify implementation

void IostreamReadStateSmallStep::Execute(TaskContext &ctx) {
  if (ctx.GetTaskBuilder().progress_) {
    io_facility_->Print(IostreamStrings::kInvalidState);
    io_facility_->Print(IostreamStrings::LeaveEmptyFor(
        IostreamStrings::to_string(*ctx.GetTaskBuilder().progress_)));
  }
  std::string prompt = IostreamStrings::GetPrompt("state");
  auto progress_string = PrintAndGet(*io_facility_, prompt);
  if (progress_string.empty()) {
    ctx.PopState();
    return;
  }
  auto validated_state = validator_->ParseTaskProgress(progress_string);
  for (; !validated_state;) {
    io_facility_->Print(IostreamStrings::kInvalidState);
    progress_string = PrintAndGet(*io_facility_, prompt);
    if (progress_string.empty()) {
      ctx.PopState();
      return;
    }
    validated_state = validator_->ParseTaskProgress(progress_string);
  }
  ctx.GetTaskBuilder().progress_ = validated_state;
  ctx.PopState();
}
