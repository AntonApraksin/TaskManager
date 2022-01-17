#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_UNKNOWNREPLSTEP_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_UNKNOWNREPLSTEP_H_

#include "interactor/state_machine/interactor_steps/Step.h"

namespace task_manager {
class UnknownStep : public Step, public ArgumentMixin {
 public:
  using ArgumentMixin::ArgumentMixin;

  std::unique_ptr<Command> execute(Context ctx) override;
  void ChangeStep(std::shared_ptr<Step>&) override;

 private:
  template <int>
  std::unique_ptr<Command> HandleStage(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<0>(Context&);

  int stage_ = 1;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_UNKNOWNREPLSTEP_H_