#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_PROMPTREPLSTEP_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_PROMPTREPLSTEP_H_

#include "interactor/state_machine/interactor_steps/Step.h"
#include "interactor/state_machine/interactor_steps/StepParameter.h"

namespace task_manager {
class PromptStep : public Step, public DependenciesMixin {
 public:
  using DependenciesMixin::DependenciesMixin;

  std::unique_ptr<Command> execute(StepParameter &param) override;
  void ChangeStep(std::shared_ptr<Step> &) override;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_PROMPTREPLSTEP_H_
