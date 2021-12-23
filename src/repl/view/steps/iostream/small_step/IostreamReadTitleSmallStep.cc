#include "IostreamSmallStep.h"
#include "repl/view/steps/Strings.h"
#include "repl/view/steps/TaskContext.h"

// TODO: Prettify implementation

namespace task_manager {
void IostreamReadTitleSmallStep::Execute(TaskContext &ctx) {
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