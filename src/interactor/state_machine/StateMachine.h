#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_STATEMACHINE_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_STATEMACHINE_H_

#include "interactor/state_machine/Context.h"
#include "interactor/state_machine/commands/Commands.h"
#include "interactor/state_machine/interactor_steps/Step.h"

namespace task_manager
{
class StateMachine
{
public:
    StateMachine(std::shared_ptr<IValidator>, std::shared_ptr<IIoFacility>,
                 std::shared_ptr<ISmallStepFactory>);
    std::unique_ptr<Command> execute(Context ctx);

private:
    std::shared_ptr<IValidator> validator_;
    std::shared_ptr<IIoFacility> io_facility_;
    std::shared_ptr<ISmallStepFactory> small_step_factory_;
    std::shared_ptr<Step> active_step_;
    StepParameter step_parameter_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_STATEMACHINE_H_
