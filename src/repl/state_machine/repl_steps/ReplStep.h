#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_REPLSTEP_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_REPLSTEP_H_

#include "repl/io_facility/IIoFacility.h"
#include "repl/state_machine/commands/Commands.h"
#include "repl/validator/IValidator.h"

namespace task_manager {

class ISmallStepFactory;

class ReplStep {
 public:
  virtual std::unique_ptr<Command> execute(Context ctx) = 0;
  virtual void ChangeStep(std::shared_ptr<ReplStep>&) = 0;
  virtual ~ReplStep() {}
};

class DependenciesMixin {
 public:
  DependenciesMixin(std::shared_ptr<IValidator> validator,
                    std::shared_ptr<IIoFacility> io_facility,
                    std::shared_ptr<ISmallStepFactory> small_step_factory)
      : validator_(std::move(validator)),
        io_facility_(std::move(io_facility)),
        small_step_factory_(std::move(small_step_factory)) {}

 protected:
  ~DependenciesMixin() {}

  std::shared_ptr<IValidator> validator_;
  std::shared_ptr<IIoFacility> io_facility_;
  std::shared_ptr<ISmallStepFactory> small_step_factory_;
};

class ArgumentMixin : public DependenciesMixin {
 public:
  ArgumentMixin(std::shared_ptr<IValidator> validator,
                std::shared_ptr<IIoFacility> io_facility,
                std::shared_ptr<ISmallStepFactory> small_step_factory,
                std::string arg)
      : DependenciesMixin(std::move(validator), std::move(io_facility),
                          std::move(small_step_factory)),
        arg_(std::move(arg)) {}

 protected:
  ~ArgumentMixin() {}

  std::string arg_;
};

}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_REPLSTEP_H_
