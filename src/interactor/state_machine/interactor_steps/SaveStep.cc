#include "interactor/state_machine/interactor_steps/SaveStep.h"

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/FinalizeStep.h"

namespace task_manager
{
std::unique_ptr<Command> SaveStep::execute(StepParameter& param)
{
    param.ctx.event = StepEvent::kNothing;
    if (!arg_.empty())
    {
        return ReportError(Strings::kMultipleArgumentDoesNotSupported);
    }
    return std::make_unique<SaveTasksCommand>();
}

std::shared_ptr<Step> SaveStep::ChangeStep()
{
    return std::make_shared<FinalizeStep>(validator_, io_facility_,
                                          small_step_factory_);
}

std::unique_ptr<Command> SaveStep::ReportError(std::string str)
{
    io_facility_->Print(str);
    return std::make_unique<VoidCommand>();
}
}  // namespace task_manager
