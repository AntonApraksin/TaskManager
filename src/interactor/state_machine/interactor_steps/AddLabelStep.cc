#include "AddLabelStep.h"

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/FinalizeStep.h"
#include "interactor/state_machine/interactor_steps/utils/IoFacilityAndValidatorUtils.h"
#include "interactor/state_machine/interactor_steps/utils/ValidatorUtils.h"
#include "utils/Functions.h"

namespace task_manager
{

std::unique_ptr<Command> AddLabelStep::execute(StepParameter& param)
{
    param.ctx.event = StepEvent::kNothing;
    if (arg_.empty())
    {
        return ReportError(Strings::kRequiredId);
    }

    // Reading Id
    auto task_id = ConsumeTaskIdFromString(*validator_, arg_);
    if (!task_id)
    {
        return ReportError(Strings::kInvalidId);
    }

    // Reading Label
    if (arg_.empty())
    {
        return ReportError(Strings::kRequiredLabel);
    }

    auto label_name = validator_->ConsumeOneTokenFrom(arg_);
    if (!arg_.empty())
    {
        return ReportError(Strings::kMultipleArgumentDoesNotSupported);
    }

    Label label;
    label.set_name(label_name);

    auto found = FindSolidTaskById(param.cache, *task_id);
    if (found)
    {
        io_facility_->Print(Strings::YouAreGoingTo("add label to"));
        io_facility_->Print(Strings::ShowSolidTask(*found));
    }

    auto confirm = ReadConfirmation(*io_facility_, *validator_, "add label");
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
    return std::make_unique<AddLabelCommand>(*task_id, label);
}

std::shared_ptr<Step> AddLabelStep::ChangeStep()
{
    return std::make_shared<FinalizeStep>(validator_, io_facility_,
                                          small_step_factory_);
}

std::unique_ptr<Command> AddLabelStep::ReportError(std::string str)
{
    io_facility_->Print(str);
    return std::make_unique<VoidCommand>();
}
}  // namespace task_manager