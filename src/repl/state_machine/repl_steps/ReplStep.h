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

class ValidatorAndIoFacilityMixin {
 public:
  ValidatorAndIoFacilityMixin(std::shared_ptr<IValidator> validator,
                              std::shared_ptr<IIoFacility> io_facility)
      : validator_(std::move(validator)),
        io_facility_(std::move(io_facility)) {}

 protected:
  ~ValidatorAndIoFacilityMixin() {}

  std::shared_ptr<IValidator> validator_;
  std::shared_ptr<IIoFacility> io_facility_;
};

class SmallStepMixin {
 public:
  explicit SmallStepMixin(
      const std::shared_ptr<ISmallStepFactory>& small_step_factory)
      : small_step_factory_(small_step_factory) {}

 protected:
  ~SmallStepMixin() {}

  std::shared_ptr<ISmallStepFactory> small_step_factory_;
};

class ArgumentMixin : public ValidatorAndIoFacilityMixin {
 public:
  ArgumentMixin(std::shared_ptr<IValidator> validator,
                std::shared_ptr<IIoFacility> io_facility, std::string arg)
      : ValidatorAndIoFacilityMixin(std::move(validator),
                                    std::move(io_facility)),
        arg_(std::move(arg)) {}

 protected:
  ~ArgumentMixin() {}

  std::string arg_;
};

}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_REPL_STEPS_REPLSTEP_H_
