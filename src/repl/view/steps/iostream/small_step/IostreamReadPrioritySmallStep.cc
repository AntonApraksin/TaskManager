#include "IostreamSmallStep.h"
#include "repl/view/steps/Strings.h"
#include "repl/view/steps/TaskContext.h"

// TODO: Prettify implementation

namespace task_manager {
void IostreamReadPrioritySmallStep::Execute(TaskContext &ctx) {
  if (ctx.GetTaskBuilder().priority_) {
    io_facility_->Print(Strings::LeaveEmptyFor(
        Strings::to_string(*ctx.GetTaskBuilder().priority_)));
  }
  std::string prompt = Strings::GetPrompt("priority");
  auto priority_string = PrintAndGet(*io_facility_, prompt);
  if (priority_string.empty()) {
    ctx.PopState();
    return;
  }
  auto validated_priority = validator_->ParseTaskPriority(priority_string);
  for (; !validated_priority;) {
    io_facility_->Print(Strings::kPriorityShouldBe);
    priority_string = PrintAndGet(*io_facility_, prompt);
    if (priority_string.empty()) {
      ctx.PopState();
      return;
    }
    validated_priority = validator_->ParseTaskPriority(priority_string);
  }
  ctx.GetTaskBuilder().priority_ = validated_priority;
  ctx.PopState();
}
}  // namespace task_manager