#ifndef TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_INTERACTOR_STEPS_DELETELABELSTEP_H_
#define TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_INTERACTOR_STEPS_DELETELABELSTEP_H_

#include "interactor/state_machine/interactor_steps/Step.h"

namespace task_manager
{
class DeleteLabelStep : public Step, public ArgumentMixin
{
public:
    using ArgumentMixin::ArgumentMixin;

    std::unique_ptr<Command> execute(StepParameter&) override;
    std::shared_ptr<Step> ChangeStep() override;

private:
    std::unique_ptr<Command> ReportError(std::string);
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_INTERACTOR_STEPS_DELETELABELSTEP_H_
