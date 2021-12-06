#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEPFACTORY_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEPFACTORY_H_

#include "IStep.h"

class IStepFactory {
 public:
  virtual std::shared_ptr<IStep> GetAddTaskREPLState() = 0;
  virtual std::shared_ptr<IStep> GetAddTaskREPLState(Task) = 0;

  virtual std::shared_ptr<IStep> GetEditTaskREPLState(
      IEditTaskStep::TaskWrapperRef) = 0;
  virtual std::shared_ptr<IStep> GetCompleteTaskREPLState(
      ICompleteTaskStep::TaskWrappers) = 0;
  virtual std::shared_ptr<IStep> GetDeleteTaskREPLState(
      IDeleteTaskStep::TaskWrappers) = 0;

  virtual std::shared_ptr<IStep> GetShowAllTasksREPLState(
      IShowAllTasksStep::TaskStorageRef) = 0;
  virtual std::shared_ptr<IStep> GetShowNTasksREPLState(
      IShowNTasksStep::TaskWrappers) = 0;
  virtual std::shared_ptr<IStep> GetShowSortedTasksREPLState(
      IShowSortedTasksStep::Tasks) = 0;

  virtual ~IStepFactory() {}
};

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEPFACTORY_H_
