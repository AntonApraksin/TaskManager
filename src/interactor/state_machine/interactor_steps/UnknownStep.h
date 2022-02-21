#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_UNKNOWNREPLSTEP_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_UNKNOWNREPLSTEP_H_

#include "interactor/state_machine/interactor_steps/Step.h"

namespace task_manager
{
class UnknownStep : public Step, public ArgumentMixin
{
public:
    using ArgumentMixin::ArgumentMixin;

    std::unique_ptr<Command> execute(StepParameter& param) override;
    std::shared_ptr<Step> ChangeStep() override;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_UNKNOWNREPLSTEP_H_
