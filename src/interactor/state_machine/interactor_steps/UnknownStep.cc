#include "interactor/state_machine/interactor_steps/UnknownStep.h"

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/FinalizeStep.h"

namespace task_manager {
std::unique_ptr<Command> UnknownStep::execute(StepParameter &param) {
  io_facility_->Print(Strings::kUnknownCommand);
  param.ctx.event = StepEvent::kNothing;
  return std::make_unique<VoidCommand>();
}

std::shared_ptr<Step> UnknownStep::ChangeStep() {
  return std::make_shared<FinalizeStep>(validator_, io_facility_,
                                        small_step_factory_);
}
}  // namespace task_manager
