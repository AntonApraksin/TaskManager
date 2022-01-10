#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_PROMPTREPLSTEP_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_PROMPTREPLSTEP_H_

#include "repl/state_machine/repl_steps/ReplStep.h"

namespace task_manager {
class PromptReplStep : public ReplStep, public DependenciesMixin {
 public:
  using DependenciesMixin::DependenciesMixin;

  std::unique_ptr<Command> execute(Context ctx) override;
  void ChangeStep(std::shared_ptr<ReplStep>&) override;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_PROMPTREPLSTEP_H_
