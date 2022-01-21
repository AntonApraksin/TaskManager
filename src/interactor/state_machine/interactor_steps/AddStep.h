#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_ADDREPLSTEP_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_ADDREPLSTEP_H_

#include "interactor/state_machine/interactor_steps/Step.h"

namespace task_manager {
class AddStep : public Step, public ArgumentMixin {
 public:
  using ArgumentMixin::ArgumentMixin;

  std::unique_ptr<Command> execute(StepParameter&) override;
  void ChangeStep(std::shared_ptr<Step>&) override;

 private:
  std::unique_ptr<Command> HandleAddTask(StepParameter&);
  std::unique_ptr<Command> HandleAddSubTask(StepParameter&);

  std::unique_ptr<Command> ReportError(std::string);

  std::optional<TaskId> task_id_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_ADDREPLSTEP_H_
