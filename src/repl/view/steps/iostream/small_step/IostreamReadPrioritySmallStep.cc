#include "IostreamSmallStep.h"
#include "repl/view/steps/TaskContext.h"
#include "repl/view/steps/iostream/IostreamStrings.h"

// TODO: Prettify implementation

void IostreamReadPrioritySmallStep::Execute(TaskContext &ctx) {
  if (ctx.GetTaskBuilder().priority_) {
    io_facility_->Print(IostreamStrings::LeaveEmptyFor(
        IostreamStrings::to_string(*ctx.GetTaskBuilder().priority_)));
  }
  std::string prompt = IostreamStrings::GetPrompt("priority");
  auto priority_string = PrintAndGet(*io_facility_, prompt);
  if (priority_string.empty()) {
    ctx.PopState();
    return;
  }
  auto validated_priority = validator_->ParseTaskPriority(priority_string);
  for (; !validated_priority;) {
    io_facility_->Print(IostreamStrings::kInvalidPriority);
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