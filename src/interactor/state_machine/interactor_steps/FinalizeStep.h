#ifndef TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_INTERACTOR_STEPS_FINALIZESTEP_H_
#define TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_INTERACTOR_STEPS_FINALIZESTEP_H_

#include "interactor/state_machine/interactor_steps/Step.h"

namespace task_manager
{
class FinalizeStep : public Step, private DependenciesMixin
{
public:
    using DependenciesMixin::DependenciesMixin;

    std::unique_ptr<Command> execute(StepParameter&) override;
    std::shared_ptr<Step> ChangeStep() override;

private:
    std::unique_ptr<Command> ShowAll(StepParameter&);
    std::unique_ptr<Command> ShowId(StepParameter&);
    std::unique_ptr<Command> ShowSpecificTasks(StepParameter&);
    std::unique_ptr<Command> ShowWithoutNesting(StepParameter&);
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_INTERACTOR_STEPS_FINALIZESTEP_H_
