#include "interactor/state_machine/interactor_steps/HelpStep.h"

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/FinalizeStep.h"

namespace task_manager
{
std::unique_ptr<Command> HelpStep::execute(StepParameter& param)
{
    param.ctx.event = StepEvent::kNothing;
    io_facility_->Print(Strings::kHelp);
    return std::make_unique<VoidCommand>();
}

std::shared_ptr<Step> HelpStep::ChangeStep()
{
    return std::make_shared<FinalizeStep>(validator_, io_facility_,
                                          small_step_factory_);
}
}  // namespace task_manager