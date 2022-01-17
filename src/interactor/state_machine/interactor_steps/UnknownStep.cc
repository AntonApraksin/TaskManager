#include "interactor/state_machine/interactor_steps/UnknownStep.h"

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/PromptStep.h"

namespace task_manager {
std::unique_ptr<Command> UnknownStep::execute(Context ctx) {
  --stage_;
  if (stage_ == 0) {
    return HandleStage<0>(ctx);
  }
  std::terminate();
}

template <>
std::unique_ptr<Command> UnknownStep::HandleStage<0>(Context &) {
  io_facility_->Print(Strings::kUnknownCommand);
  return std::make_unique<VoidCommand>();
}

void UnknownStep::ChangeStep(std::shared_ptr<Step> &active_step) {
  if (stage_ == 0) {
    active_step = std::make_shared<PromptStep>(validator_, io_facility_,
                                               small_step_factory_);
  }
}
}  // namespace task_manager