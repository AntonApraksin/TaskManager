#include <sstream>

#include "IostreamSmallStep.h"
#include "repl/view/steps/TaskContext.h"
#include "repl/view/steps/iostream/IostreamGeneralFunctional.h"

// TODO: Prettify implementation

void IostreamReadPrioritySmallStep::Execute(TaskContext &ctx) {
  std::stringstream ss;
  if (ctx.GetTaskBuilder().priority_) {
    ss << "Leave empty for '" << to_string(*ctx.GetTaskBuilder().priority_)
       << "'\n";
    io_facility_->Print(ss.str());
    ss.str("");
  }
  auto priority_string = PrintAndGet(*io_facility_, "priority");
  if (priority_string.empty()) {
    ctx.PopState();
    return;
  }
  auto validated_priority = validator_->ParseTaskPriority(priority_string);
  for (; !validated_priority;) {
    ss << "Priority should be: low, medium, high.\n";
    io_facility_->Print(ss.str());
    ss.str("");
    priority_string = PrintAndGet(*io_facility_, "priority");
    if (priority_string.empty()) {
      ctx.PopState();
      return;
    }
    validated_priority = validator_->ParseTaskPriority(priority_string);
  }
  ctx.GetTaskBuilder().priority_ = validated_priority;
  ctx.PopState();
}