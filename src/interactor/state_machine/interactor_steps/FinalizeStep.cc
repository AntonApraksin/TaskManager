#include "FinalizeStep.h"

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/PromptStep.h"

namespace task_manager
{
std::unique_ptr<Command> FinalizeStep::execute(StepParameter& param)
{
    switch (param.ctx.event)
    {
        case StepEvent::kShowId: { return ShowId(param); }

        case StepEvent::kNotPresentLabel:
        {
            io_facility_->Print(Strings::kNotPresentLabel);
            return std::make_unique<VoidCommand>();
        }

        case StepEvent::kNotPresentId:
        {
            io_facility_->Print(Strings::kNotPresentId);
            return std::make_unique<VoidCommand>();
        }

        case StepEvent::kLoadFailure:
        {
            io_facility_->Print(Strings::kLoadFailure);
            return std::make_unique<VoidCommand>();
        }

        case StepEvent::kSaveFailure:
        {
            io_facility_->Print(Strings::kSaveFailure);
            return std::make_unique<VoidCommand>();
        }

        case StepEvent::kShowAll: { return ShowAll(param); }

        case StepEvent::kShowById: { return ShowSpecificTasks(param); }

        case StepEvent::kShowWithoutNest: { return ShowWithoutNesting(param); }

        case StepEvent::kNothing: { return std::make_unique<VoidCommand>(); }
    }
}

std::unique_ptr<Command> FinalizeStep::ShowAll(StepParameter& param)
{
    io_facility_->Print(Strings::ShowSolidTasks(*param.ctx.solid_tasks));
    param.cache = std::move(*param.ctx.solid_tasks);
    param.ctx.solid_tasks.reset();
    return std::make_unique<VoidCommand>();
}

std::unique_ptr<Command> FinalizeStep::ShowId(StepParameter& param)
{
    io_facility_->Print(Strings::ShowId(std::to_string(param.ctx.task_id->id())));
    return std::make_unique<VoidCommand>();
}

std::unique_ptr<Command> FinalizeStep::ShowSpecificTasks(StepParameter& param)
{
    io_facility_->Print(Strings::ShowSolidTasks(*param.ctx.solid_tasks));
    param.ctx.solid_tasks.reset();
    return std::make_unique<VoidCommand>();
}

std::shared_ptr<Step> FinalizeStep::ChangeStep()
{
    return std::make_shared<PromptStep>(validator_, io_facility_,
                                        small_step_factory_);
}

std::unique_ptr<Command> FinalizeStep::ShowWithoutNesting(
    StepParameter& param)
{
    io_facility_->Print(
        Strings::ShowSolidTasksWithoutNest(*param.ctx.solid_tasks));
    param.ctx.solid_tasks.reset();
    return std::make_unique<VoidCommand>();
}
}  // namespace task_manager
