#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_SAVEREPLSTEP_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_SAVEREPLSTEP_H_

#include "interactor/state_machine/interactor_steps/Step.h"

namespace task_manager {
class SaveStep : public Step, public ArgumentMixin {
 public:
  using ArgumentMixin::ArgumentMixin;

  std::unique_ptr<Command> execute(StepParameter &param) override;
  void ChangeStep(std::shared_ptr<Step> &) override;

 private:
  std::unique_ptr<Command> ReportError(std::string);

  std::string filename_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_SAVEREPLSTEP_H_
