#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_EDITREPLSTEP_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_EDITREPLSTEP_H_

#include "repl/state_machine/repl_steps/ReplStep.h"

namespace task_manager {
class EditReplStep : public ReplStep, public ArgumentMixin {
 public:
  using ArgumentMixin::ArgumentMixin;

  std::unique_ptr<Command> execute(Context ctx) override;
  void ChangeStep(std::shared_ptr<ReplStep>&) override;

 private:
  template <int>
  std::unique_ptr<Command> HandleStage(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<1>(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<0>(Context&);

  std::unique_ptr<Command> ReportError(std::string);

  TaskId task_id_;
  int stage_ = 2;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_EDITREPLSTEP_H_
