#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_ADDREPLSTEP_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_ADDREPLSTEP_H_

#include "repl/state_machine/repl_steps/ReplStep.h"

namespace task_manager {
class AddReplStep : public ReplStep, public ArgumentMixin {
 public:
  using ArgumentMixin::ArgumentMixin;

  std::unique_ptr<Command> execute(Context) override;
  void ChangeStep(std::shared_ptr<ReplStep>&) override;

 private:
  template <int>
  std::unique_ptr<Command> HandleStage(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<2>(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<1>(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<0>(Context&);

  std::unique_ptr<Command> HandleAddTask(Context&);
  std::unique_ptr<Command> HandleAddSubTask(Context&);

  std::unique_ptr<Command> ReportError(std::string);

  int stage_ = 3;
  std::optional<TaskId> task_id_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_ADDREPLSTEP_H_
