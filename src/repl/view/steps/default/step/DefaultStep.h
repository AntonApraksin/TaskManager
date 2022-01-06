#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEP_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEP_H_

#include "repl/io_facility/IIoFacility.h"
#include "repl/view/steps/IStep.h"

namespace task_manager {
class ISmallStepFactory;

class DefaultStep {
 protected:
  explicit DefaultStep(const std::shared_ptr<IIoFacility>& io_facility)
      : io_facility_(io_facility) {}
  ~DefaultStep() {}

  std::shared_ptr<IIoFacility> io_facility_;
};

class DefaultWithSmallStepStep {
 protected:
  explicit DefaultWithSmallStepStep(
      const std::shared_ptr<ISmallStepFactory>& small_step_factory)
      : small_step_factory_(small_step_factory) {}

  ~DefaultWithSmallStepStep() {}
  std::shared_ptr<ISmallStepFactory> small_step_factory_;
};

class DefaultWithValidatorStep {
 protected:
  explicit DefaultWithValidatorStep(
      const std::shared_ptr<IValidator>& validator)
      : validator_(validator) {}
  ~DefaultWithValidatorStep() {}

  std::shared_ptr<IValidator> validator_;
};

class DefaultAddTaskStep final : public IAddTaskStep,
                                 public DefaultStep,
                                 public DefaultWithSmallStepStep,
                                 public DefaultWithValidatorStep {
 public:
  DefaultAddTaskStep(const std::shared_ptr<IIoFacility>&,
                     const std::shared_ptr<ISmallStepFactory>&,
                     const std::shared_ptr<IValidator>&);

  StepResult Run() override;
};

class DefaultEditTaskStep final : public IEditTaskStep,
                                  public DefaultStep,
                                  public DefaultWithSmallStepStep,
                                  public DefaultWithValidatorStep {
 public:
  DefaultEditTaskStep(const std::shared_ptr<IIoFacility>&,
                      const std::shared_ptr<ISmallStepFactory>&,
                      const std::shared_ptr<IValidator>&);

  StepResult Run() override;
};

class DefaultDeleteTaskStep final : public IDeleteTaskStep,
                                    public DefaultStep,
                                    public DefaultWithValidatorStep {
 public:
  DefaultDeleteTaskStep(const std::shared_ptr<IIoFacility>&,
                        const std::shared_ptr<IValidator>&);

  StepResult Run() override;
};

class DefaultCompleteTaskStep final : public ICompleteTaskStep,
                                      public DefaultStep,
                                      public DefaultWithValidatorStep {
 public:
  DefaultCompleteTaskStep(const std::shared_ptr<IIoFacility>&,
                          const std::shared_ptr<IValidator>&);

  StepResult Run() override;
};

class DefaultShowStep final : public IShowStep, public DefaultStep {
 public:
  explicit DefaultShowStep(const std::shared_ptr<IIoFacility>&);
  StepResult Run() override;
};

class DefaultShowHelpStep final : public IShowHelpStep, public DefaultStep {
 public:
  explicit DefaultShowHelpStep(const std::shared_ptr<IIoFacility>&);
  StepResult Run() override;
};

class DefaultReportMessageStep final : public IReportMessageStep,
                                       public DefaultStep {
 public:
  explicit DefaultReportMessageStep(const std::shared_ptr<IIoFacility>&);
  StepResult Run() override;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEP_H_