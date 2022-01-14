#include "DefaultSmallStep.h"
#include "repl/io_facility/Strings.h"
#include "repl/small_steps/TaskContext.h"

namespace task_manager {
void DefaultReadTitleSmallStep::Execute(TaskContext &ctx) {
  std::string prompt = Strings::GetPrompt("title");
  if (ctx.GetTaskBuilder().title_) {
    io_facility_->Print(Strings::LeaveEmptyFor(*ctx.GetTaskBuilder().title_));
    auto title = validator_->ValidateTitle(PrintAndGet(*io_facility_, prompt));
    if (title) {
      ctx.GetTaskBuilder().title_ = std::move(title);
    }
    ctx.PopState();
    return;
  }
  auto title = validator_->ValidateTitle(PrintAndGet(*io_facility_, prompt));
  for (; !title;) {
    io_facility_->Print(Strings::kTitleMustNotBeEmpty);
    title = validator_->ValidateTitle(PrintAndGet(*io_facility_, prompt));
  }
  ctx.GetTaskBuilder().title_ = std::move(title);
  ctx.PopState();
}
}  // namespace task_manager