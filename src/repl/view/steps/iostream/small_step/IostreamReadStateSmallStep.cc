#include <iostream>

#include "IostreamSmallStep.h"
#include "repl/view/steps/TaskContext.h"
#include "repl/view/steps/iostream/IostreamGeneralFunctional.h"

// TODO: Prettify implementation

void IostreamReadStateSmallStep::Execute(TaskContext &ctx) {
  if (ctx.GetTaskBuilder().state_) {
    std::cout << "Leave empty for '" << to_string(*ctx.GetTaskBuilder().state_)
              << "'\n";
  }
  auto state_string = PrintAndGet("state");
  if (state_string.empty()) {
    ctx.PopState();
    return;
  }
  auto validated_state = validator_->ParseTaskState(state_string);
  for (; !validated_state;) {
    std::cout << "Priority should be: low, medium, high.\n";
    state_string = PrintAndGet("priority");
    if (state_string.empty()) {
      ctx.PopState();
      return;
    }
    validated_state = validator_->ParseTaskState(state_string);
  }
  ctx.GetTaskBuilder().state_ = validated_state;
  ctx.PopState();
}
