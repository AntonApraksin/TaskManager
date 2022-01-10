#include "DefaultSmallStep.h"
#include "repl/io_facility/Strings.h"
#include "repl/view/steps/TaskContext.h"

namespace task_manager {
void DefaultReadStateSmallStep::Execute(TaskContext &ctx) {
  if (ctx.GetTaskBuilder().progress_) {
    io_facility_->Print(Strings::kStateShouldBe);
    io_facility_->Print(Strings::LeaveEmptyFor(
        Strings::to_string(*ctx.GetTaskBuilder().progress_)));
  }
  std::string prompt = Strings::GetPrompt("state");
  auto progress_string = PrintAndGet(*io_facility_, prompt);
  if (progress_string.empty()) {
    ctx.PopState();
    return;
  }
  auto validated_state = validator_->ParseTaskProgress(progress_string);
  for (; !validated_state;) {
    io_facility_->Print(Strings::kStateShouldBe);
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
}  // namespace task_manager