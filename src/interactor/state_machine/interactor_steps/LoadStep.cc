#include "interactor/state_machine/interactor_steps/LoadStep.h"

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/FinalizeStep.h"

namespace task_manager
{
std::unique_ptr<Command> LoadStep::execute(StepParameter& param)
{
    param.ctx.event = StepEvent::kNothing;
    if (!arg_.empty())
    {
        return ReportError(Strings::kMultipleArgumentDoesNotSupported);
    }

    param.cache.clear();
    return std::make_unique<LoadTasksCommand>();
}

std::shared_ptr<Step> LoadStep::ChangeStep()
{
    return std::make_shared<FinalizeStep>(validator_, io_facility_,
                                          small_step_factory_);
}

std::unique_ptr<Command> LoadStep::ReportError(std::string str)
{
    io_facility_->Print(str);
    return std::make_unique<VoidCommand>();
}
}  // namespace task_manager
