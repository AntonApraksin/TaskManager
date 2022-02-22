#include "StateMachine.h"

#include "interactor/state_machine/interactor_steps/PromptStep.h"

namespace task_manager
{
StateMachine::StateMachine(
    std::shared_ptr<IValidator> validator,
    std::shared_ptr<IIoFacility> io_facility,
    std::shared_ptr<ISmallStepFactory> small_step_factory)
    : validator_(std::move(validator)),
      io_facility_(std::move(io_facility)),
      small_step_factory_(std::move(small_step_factory)),
      active_step_(std::make_shared<PromptStep>(validator_, io_facility_,
                                                small_step_factory_)) {}

std::unique_ptr<Command> StateMachine::execute(Context ctx)
{
    if (!active_step_)
    {
        return {};
    }
    if (ctx.event == StepEvent::kNothing)
    {
        ctx.event = step_parameter_.ctx.event;
    }
    step_parameter_.ctx = std::move(ctx);
    auto command{active_step_->execute(step_parameter_)};
    auto next_step = active_step_->ChangeStep();
    active_step_ = next_step;
    return command;
}
}  // namespace task_manager
