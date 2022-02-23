#include "interactor/state_machine/interactor_steps/ShowStep.h"

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/FinalizeStep.h"
#include "interactor/state_machine/interactor_steps/utils/ValidatorUtils.h"
#include "utils/TaskIdUtils.h"

namespace task_manager
{
std::unique_ptr<Command> ShowStep::execute(StepParameter& param)
{
    if (arg_.empty())
    {
        param.ctx.event = StepEvent::kShowAll;
        return std::make_unique<GetAllTasksCommand>();
    }

    auto token = validator_->ConsumeOneTokenFrom(arg_);
    auto possible_id = validator_->ParseInt(token);
    if (!possible_id)
    {
        if (!arg_.empty())
        {
            return ReportError(Strings::kMultipleArgumentsAreNotSupported);
        }
        Label label;
        label.set_name(token);
        param.ctx.event = StepEvent::kShowWithoutNest;
        return std::make_unique<GetTasksByLabelCommand>(std::move(label));
    }

    std::set<TaskId> task_ids;
    task_ids.insert(CreateTaskId(*possible_id));
    for (; !arg_.empty() ;)
    {
        auto task_id = ConsumeTaskIdFromString(*validator_, arg_);
        if (!task_id)
        {
            return ReportError(Strings::kInvalidId);
        }
        task_ids.insert(*task_id);
    }

    param.ctx.event = StepEvent::kShowById;
    return std::make_unique<GetSpecifiedTasksCommand>(
        std::vector<TaskId>(task_ids.cbegin(), task_ids.cend()));
}

std::shared_ptr<Step> ShowStep::ChangeStep()
{
    return std::make_shared<FinalizeStep>(validator_, io_facility_,
                                          small_step_factory_);
}

std::unique_ptr<Command> ShowStep::ReportError(std::string str)
{
    io_facility_->Print(str);
    return std::make_unique<VoidCommand>();
}
}  // namespace task_manager
