#include "interactor/state_machine/interactor_steps/HelpStep.h"

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/PromptStep.h"

namespace task_manager {
std::unique_ptr<Command> HelpStep::execute(Context ctx) {
  --stage_;
  if (stage_ == 0) {
    return HandleStage<0>(ctx);
  }
  std::terminate();
}

template <>
std::unique_ptr<Command> HelpStep::HandleStage<0>(Context &) {
  io_facility_->Print(Strings::kHelp);
  return std::make_unique<VoidCommand>();
}

void HelpStep::ChangeStep(std::shared_ptr<Step> &active_step) {
  if (stage_ == 0) {
    active_step = std::make_shared<PromptStep>(validator_, io_facility_,
                                               small_step_factory_);
  }
}
}  // namespace task_manager