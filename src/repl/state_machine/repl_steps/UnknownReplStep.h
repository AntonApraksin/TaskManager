#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_UNKNOWNREPLSTEP_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_UNKNOWNREPLSTEP_H_

#include "repl/state_machine/repl_steps/ReplStep.h"

namespace task_manager {
class UnknownReplStep : public ReplStep,
                        public ArgumentMixin,
                        public SmallStepMixin {
 public:
  UnknownReplStep(std::shared_ptr<IValidator> validator,
                  std::shared_ptr<IIoFacility> io_facility,
                  std::shared_ptr<ISmallStepFactory> small_step_factory,
                  std::string arg)
      : ArgumentMixin(std::move(validator), std::move(io_facility),
                      std::move(arg)),
        SmallStepMixin(std::move(small_step_factory)) {}

  std::unique_ptr<Command> execute(Context ctx) override;
  void ChangeStep(std::shared_ptr<ReplStep>&) override;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_UNKNOWNREPLSTEP_H_
