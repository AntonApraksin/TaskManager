#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_EDITREPLSTEP_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_EDITREPLSTEP_H_

#include "interactor/state_machine/interactor_steps/Step.h"

namespace task_manager {
class EditStep : public Step, public ArgumentMixin {
 public:
  using ArgumentMixin::ArgumentMixin;

  std::unique_ptr<Command> execute(StepParameter &param) override;
  void ChangeStep(std::shared_ptr<Step> &) override;

 private:
  std::unique_ptr<Command> ReportError(std::string);
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_EDITREPLSTEP_H_
