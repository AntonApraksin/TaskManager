#include <iostream>

#include "IostreamSmallStep.h"
#include "repl/view/small_step/TaskContext.h"
#include "repl/view/small_step/iostream_small_step/IostreamSmallStepGeneralFunctional.h"
#include "repl/view/step/iostream_step/IostreamGeneralFunctional.h"

// TODO: Prettify implementation

void IostreamReadPrioritySmallStep::Execute(TaskContext &ctx) {
  if (ctx.GetTaskBuilder().priority_) {
    std::cout << "Leave empty for '"
              << to_string(*ctx.GetTaskBuilder().priority_) << "'\n";
  }
  auto priority_string = PrintAndGet("priority");
  if (priority_string.empty()) {
    ctx.PopState();
    return;
  }
  auto validated_priority = validator_->ParseTaskPriority(priority_string);
  for (; !validated_priority;) {
    std::cout << "Priority should be: low, medium, high.\n";
    priority_string = PrintAndGet("priority");
    if (priority_string.empty()) {
      ctx.PopState();
      return;
    }
    validated_priority = validator_->ParseTaskPriority(priority_string);
  }
  ctx.GetTaskBuilder().priority_ = validated_priority;
  ctx.PopState();
}