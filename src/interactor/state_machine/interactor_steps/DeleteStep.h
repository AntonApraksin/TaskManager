#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_DELETEREPLSTEP_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_DELETEREPLSTEP_H_

#include "interactor/state_machine/interactor_steps/Step.h"

namespace task_manager {
class DeleteStep : public Step, public ArgumentMixin {
 public:
  using ArgumentMixin::ArgumentMixin;

  std::unique_ptr<Command> execute(StepParameter &param) override;
  void ChangeStep(std::shared_ptr<Step> &) override;

 private:
  std::unique_ptr<Command> ReportError(std::string);

  TaskId task_id_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_DELETEREPLSTEP_H_
