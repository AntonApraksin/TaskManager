#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEP_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEP_H_

#include "repl/view/steps/IStep.h"

// TODO: Give better names

class ISmallStepFactory;

class IostreamStep {
 protected:
  explicit IostreamStep(
      const std::shared_ptr<ISmallStepFactory>& state_factory)
      : state_factory_(state_factory) {}
  ~IostreamStep() {}

  std::shared_ptr<ISmallStepFactory> state_factory_;
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
                                  public IostreamWithValidatorStep {
 public:
  IostreamAddTaskStep(const std::shared_ptr<ISmallStepFactory>&,
                      const std::shared_ptr<IValidator>&);

  StepResult Run() override;
};

class IostreamEditTaskStep final : public IEditTaskStep,
                                   public IostreamStep,
                                   public IostreamWithValidatorStep {
 public:
  using IEditTaskStep::TaskWrapperRef;

  IostreamEditTaskStep(TaskWrapperRef,
                       const std::shared_ptr<ISmallStepFactory>&,
                       const std::shared_ptr<IValidator>&);

  StepResult Run() override;
};

class IostreamDeleteTaskStep final
    : public IDeleteTaskStep,
      public IostreamWithValidatorStep {
 public:
  using IDeleteTaskStep::TaskWrappers;

  IostreamDeleteTaskStep(TaskWrappers, const std::shared_ptr<IValidator>&);

  StepResult Run() override;
};

class IostreamCompleteTaskStep final
    : public ICompleteTaskStep,
      public IostreamWithValidatorStep {
 public:
  using ICompleteTaskStep::TaskWrappers;

  IostreamCompleteTaskStep(TaskWrappers,
                           const std::shared_ptr<IValidator>&);

  StepResult Run() override;
};

class IostreamShowAllTasksStep final : public IShowAllTasksStep {
 public:
  using IShowAllTasksStep::IShowAllTasksStep;
  StepResult Run() override;
};

class IostreamShowNTasksStep final : public IShowNTasksStep {
 public:
  using IShowNTasksStep::IShowNTasksStep;
  StepResult Run() override;
};

class IostreamShowSortedTasksStep final
    : public IShowSortedTasksStep {
 public:
  using IShowSortedTasksStep::IShowSortedTasksStep;
  StepResult Run() override;
};

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEP_H_
