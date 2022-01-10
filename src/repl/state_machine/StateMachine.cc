#include "StateMachine.h"

#include "repl/state_machine/repl_steps/PromptReplStep.h"

namespace task_manager {
StateMachine::StateMachine(
    std::shared_ptr<IValidator> validator,
    std::shared_ptr<IIoFacility> io_facility,
    std::shared_ptr<ISmallStepFactory> small_step_factory)
    : validator_(std::move(validator)),
      io_facility_(std::move(io_facility)),
      small_step_factory_(std::move(small_step_factory)),
      active_step_(std::make_shared<PromptReplStep>(validator_, io_facility_,
                                                    small_step_factory_)) {}

std::unique_ptr<Command> StateMachine::execute(task_manager::Context ctx) {
  if (!active_step_) {
    return {};
  }
  auto command{active_step_->execute(ctx)};
  auto next_step = active_step_;
  active_step_->ChangeStep(next_step);
  active_step_ = next_step;
  return command;
}
}  // namespace task_manager