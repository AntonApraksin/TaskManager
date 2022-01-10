#include "repl/io_facility/Strings.h"
#include "repl/state_machine/repl_steps/ReplSteps.h"

namespace task_manager {
std::unique_ptr<Command> UnknownReplStep::execute(Context) {
  io_facility_->Print(Strings::kUnknownCommand);
  return std::make_unique<VoidCommand>();
}

void UnknownReplStep::ChangeStep(std::shared_ptr<ReplSteps> &active_step) {
  active_step = std::make_shared<PromptReplStep>(validator_, io_facility_,
                                                 small_step_factory_);
}
}  // namespace task_manager
