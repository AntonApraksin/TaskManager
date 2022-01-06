#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_STATEMACHINE_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_STATEMACHINE_H_

#include "repl/state_machine/Context.h"
#include "repl/state_machine/commands/Commands.h"
#include "repl/state_machine/repl_steps/ReplSteps.h"

namespace task_manager {
class StateMachine {
 public:
  StateMachine(std::shared_ptr<IValidator>, std::shared_ptr<IIoFacility>,
               std::shared_ptr<ISmallStepFactory>);
  std::unique_ptr<Command> execute(Context ctx);

 private:
  std::shared_ptr<IValidator> validator_;
  std::shared_ptr<IIoFacility> io_facility_;
  std::shared_ptr<ISmallStepFactory> small_step_factory_;
  std::shared_ptr<ReplSteps> active_step_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_STATEMACHINE_H_
