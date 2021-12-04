#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEP_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEP_H_

#include "repl/view/steps/IStep.h"

// TODO: Give better names

class ISmallStepFactory;

class IostreamREPLState {
 protected:
  explicit IostreamREPLState(
      const std::shared_ptr<ISmallStepFactory>& state_factory)
      : state_factory_(state_factory) {}
  ~IostreamREPLState() {}

  std::shared_ptr<ISmallStepFactory> state_factory_;
};

class IostreamWithValidatorREPLState {
 protected:
  explicit IostreamWithValidatorREPLState(
      const std::shared_ptr<IValidator>& validator)
      : validator_(validator) {}
  ~IostreamWithValidatorREPLState() {}

  std::shared_ptr<IValidator> validator_;
};

class IostreamAddTaskREPLState final : public IAddTaskREPLState,
                                       public IostreamREPLState,
                                       public IostreamWithValidatorREPLState {
 public:
  IostreamAddTaskREPLState(const std::shared_ptr<ISmallStepFactory>&,
                           const std::shared_ptr<IValidator>&);

  StepResult Run() override;
};

class IostreamEditTaskREPLState final : public IEditTaskREPLState,
                                        public IostreamREPLState,
                                        public IostreamWithValidatorREPLState {
 public:
  using IEditTaskREPLState::TaskWrapperRef;

  IostreamEditTaskREPLState(TaskWrapperRef,
                            const std::shared_ptr<ISmallStepFactory>&,
                            const std::shared_ptr<IValidator>&);

  StepResult Run() override;
};

class IostreamDeleteTaskREPLState final
    : public IDeleteTaskREPLState,
      public IostreamWithValidatorREPLState {
 public:
  using IDeleteTaskREPLState::TaskWrappers;

  IostreamDeleteTaskREPLState(TaskWrappers, const std::shared_ptr<IValidator>&);

  StepResult Run() override;
};

class IostreamCompleteTaskREPLState final
    : public ICompleteTaskREPLState,
      public IostreamWithValidatorREPLState {
 public:
  using ICompleteTaskREPLState::TaskWrappers;

  IostreamCompleteTaskREPLState(TaskWrappers,
                                const std::shared_ptr<IValidator>&);

  StepResult Run() override;
};

class IostreamShowAllTasksREPLState final : public IShowAllTasksREPLState {
 public:
  using IShowAllTasksREPLState::IShowAllTasksREPLState;
  StepResult Run() override;
};

class IostreamShowNTasksREPLState final : public IShowNTasksREPLState {
 public:
  using IShowNTasksREPLState::IShowNTasksREPLState;
  StepResult Run() override;
};

class IostreamShowSortedTasksREPLState final
    : public IShowSortedTasksREPLState {
 public:
  using IShowSortedTasksREPLState::IShowSortedTasksREPLState;
  StepResult Run() override;
};

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMSTEP_H_
