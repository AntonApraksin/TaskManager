#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEPFACTORY_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEPFACTORY_H_

#include "repl/view/step/IStep.h"

class IStepFactory {
 public:
  virtual std::shared_ptr<IStep> GetAddTaskREPLState() = 0;
  virtual std::shared_ptr<IStep> GetAddTaskREPLState(Task) = 0;

  virtual std::shared_ptr<IStep> GetEditTaskREPLState(
      IEditTaskREPLState::TaskWrapperRef) = 0;
  virtual std::shared_ptr<IStep> GetCompleteTaskREPLState(
      ICompleteTaskREPLState::TaskWrappers) = 0;
  virtual std::shared_ptr<IStep> GetDeleteTaskREPLState(
      IDeleteTaskREPLState::TaskWrappers) = 0;

  virtual std::shared_ptr<IStep> GetShowAllTasksREPLState(
      IShowAllTasksREPLState::TaskStorageRef) = 0;
  virtual std::shared_ptr<IStep> GetShowNTasksREPLState(
      IShowNTasksREPLState::TaskWrappers) = 0;
  virtual std::shared_ptr<IStep> GetShowSortedTasksREPLState(
      IShowSortedTasksREPLState::Tasks) = 0;

  virtual ~IStepFactory() {}
};

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEPFACTORY_H_
