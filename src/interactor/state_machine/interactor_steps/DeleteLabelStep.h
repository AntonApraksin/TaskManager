#ifndef TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_INTERACTOR_STEPS_DELETELABELSTEP_H_
#define TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_INTERACTOR_STEPS_DELETELABELSTEP_H_

#include "interactor/state_machine/interactor_steps/Step.h"

namespace task_manager {
class DeleteLabelStep : public Step, public ArgumentMixin {
 public:
  using ArgumentMixin::ArgumentMixin;

  std::unique_ptr<Command> execute(Context) override;
  void ChangeStep(std::shared_ptr<Step>&) override;

 private:
  template <int>
  std::unique_ptr<Command> HandleStage(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<1>(Context&);

  template <>
  std::unique_ptr<Command> HandleStage<0>(Context&);

  std::unique_ptr<Command> ReportError(std::string);

  int stage_ = 2;
  TaskId task_id_;
  Label label_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_INTERACTOR_STEPS_DELETELABELSTEP_H_
