#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_HELPREPLSTEP_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_HELPREPLSTEP_H_

#include "repl/state_machine/repl_steps/ReplStep.h"

namespace task_manager {
class HelpReplStep : public ReplStep, public ArgumentMixin {
 public:
  using ArgumentMixin::ArgumentMixin;

  std::unique_ptr<Command> execute(Context ctx) override;
  void ChangeStep(std::shared_ptr<ReplStep>&) override;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_HELPREPLSTEP_H_
