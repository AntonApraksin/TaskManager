#include <sstream>

#include "IostreamSmallStep.h"
#include "repl/view/steps/TaskContext.h"
#include "repl/view/steps/iostream/IostreamGeneralFunctional.h"

// TODO: Prettify implementation

void IostreamReadStateSmallStep::Execute(TaskContext &ctx) {
  std::stringstream ss;
  if (ctx.GetTaskBuilder().state_) {
    ss << "Leave empty for '" << to_string(*ctx.GetTaskBuilder().state_)
              << "'. '-' - uncompleted, '+' - completed\n";
    io_facility_->Print(ss.str());
    ss.str("");
  }
  auto state_string = PrintAndGet(*io_facility_, "state");
  if (state_string.empty()) {
    ctx.PopState();
    return;
  }
  auto validated_state = validator_->ParseTaskState(state_string);
  for (; !validated_state;) {
    ss << "State should be '+' for completed or '-' for uncompleted\n";
    io_facility_->Print(ss.str());
    ss.str("");
    state_string = PrintAndGet(*io_facility_, "state");
    if (state_string.empty()) {
      ctx.PopState();
      return;
    }
    validated_state = validator_->ParseTaskState(state_string);
  }
  ctx.GetTaskBuilder().state_ = validated_state;
  ctx.PopState();
}
