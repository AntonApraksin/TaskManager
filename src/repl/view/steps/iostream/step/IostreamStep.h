#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEP_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEP_H_

#include "repl/io_facility/IIoFacility.h"
#include "repl/view/steps/IStep.h"

// TODO: Give better names
namespace task_manager {
class ISmallStepFactory;

class IostreamStep {
 protected:
  explicit IostreamStep(const std::shared_ptr<IIoFacility>& io_facility)
      : io_facility_(io_facility) {}
  ~IostreamStep() {}

  std::shared_ptr<IIoFacility> io_facility_;
};

class IostreamWithSmallStepStep {
 protected:
  explicit IostreamWithSmallStepStep(
      const std::shared_ptr<ISmallStepFactory>& small_step_factory)
      : small_step_factory_(small_step_factory) {}

  ~IostreamWithSmallStepStep() {}
  std::shared_ptr<ISmallStepFactory> small_step_factory_;
};

class IostreamWithValidatorStep {
 protected:
  explicit IostreamWithValidatorStep(
      const std::shared_ptr<IValidator>& validator)
      : validator_(validator) {}
  ~IostreamWithValidatorStep() {}

  std::shared_ptr<IValidator> validator_;
};

class IostreamAddTaskStep final : public IAddTaskStep,
                                  public IostreamStep,
                                  public IostreamWithSmallStepStep,
                                  public IostreamWithValidatorStep {
 public:
  IostreamAddTaskStep(const std::shared_ptr<IIoFacility>&,
                      const std::shared_ptr<ISmallStepFactory>&,
                      const std::shared_ptr<IValidator>&);

  StepResult Run() override;
};

class IostreamEditTaskStep final : public IEditTaskStep,
                                   public IostreamStep,
                                   public IostreamWithSmallStepStep,
                                   public IostreamWithValidatorStep {
 public:
  IostreamEditTaskStep(const std::shared_ptr<IIoFacility>&,
                       const std::shared_ptr<ISmallStepFactory>&,
                       const std::shared_ptr<IValidator>&);

  StepResult Run() override;
};

class IostreamDeleteTaskStep final : public IDeleteTaskStep,
                                     public IostreamStep,
                                     public IostreamWithValidatorStep {
 public:
  IostreamDeleteTaskStep(const std::shared_ptr<IIoFacility>&,
                         const std::shared_ptr<IValidator>&);

  StepResult Run() override;
};

class IostreamCompleteTaskStep final : public ICompleteTaskStep,
                                       public IostreamStep,
                                       public IostreamWithValidatorStep {
 public:
  IostreamCompleteTaskStep(const std::shared_ptr<IIoFacility>&,
                           const std::shared_ptr<IValidator>&);

  StepResult Run() override;
};

class IostreamShowStep final : public IShowStep, public IostreamStep {
 public:
  explicit IostreamShowStep(const std::shared_ptr<IIoFacility>&);
  StepResult Run() override;
};

class IostreamShowHelpStep final : public IShowHelpStep, public IostreamStep {
 public:
  explicit IostreamShowHelpStep(const std::shared_ptr<IIoFacility>&);
  StepResult Run() override;
};

class IostreamReportMessageStep final : public IReportMessageStep,
                                        public IostreamStep {
 public:
  explicit IostreamReportMessageStep(const std::shared_ptr<IIoFacility>&);
  StepResult Run() override;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEP_H_