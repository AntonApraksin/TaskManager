#include "interactor/state_machine/interactor_steps/CompleteStep.h"

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/FinalizeStep.h"
#include "interactor/state_machine/interactor_steps/utils/IoFacilityAndValidatorUtils.h"
#include "interactor/state_machine/interactor_steps/utils/ValidatorUtils.h"
#include "utils/Functions.h"

namespace task_manager
{
std::unique_ptr<Command> CompleteStep::execute(StepParameter& param)
{
    param.ctx.event = StepEvent::kNothing;

    if (arg_.empty())
    {
        return ReportError(Strings::kRequiredId);
    }

    auto task_id = ConsumeTaskIdFromString(*validator_, arg_);
    if (!arg_.empty())
    {
        return ReportError(Strings::kMultipleArgumentsAreNotSupported);
    }
    if (!task_id)
    {
        return ReportError(Strings::kInvalidId);
    }

    auto sub_tree = GetTreeFromVector(param.cache, *task_id);
    if (!sub_tree.empty())
    {
        io_facility_->Print(Strings::YouAreGoingTo("complete"));
        io_facility_->Print(Strings::ShowSolidTasks(sub_tree));
    }

    auto confirm = ReadConfirmation(*io_facility_, *validator_, "complete");

    if (!confirm)
    {
        io_facility_->Print(Strings::kOkayITreatItAsNo);
        return std::make_unique<VoidCommand>();
    }
    if (*confirm == ConfirmationResult::kNo)
    {
        return std::make_unique<VoidCommand>();
    }

    param.cache.clear();
    return std::make_unique<CompleteTasksCommand>(std::vector<TaskId>{*task_id});
}

std::shared_ptr<Step> CompleteStep::ChangeStep()
{
    return std::make_shared<FinalizeStep>(validator_, io_facility_,
                                          small_step_factory_);
}

std::unique_ptr<Command> CompleteStep::ReportError(std::string str)
{
    io_facility_->Print(str);
    return std::make_unique<VoidCommand>();
}
}  // namespace task_manager