#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEPFACTORY_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEPFACTORY_H_

#include "IStep.h"
#include "repl/view/steps/MessageEnum.h"

class IStepFactory {
 public:
  virtual std::shared_ptr<IStep> GetAddTaskStep() = 0;
  virtual std::shared_ptr<IStep> GetAddTaskStep(Task) = 0;

  virtual std::shared_ptr<IStep> GetEditTaskStep(
      IEditTaskStep::TaskWrapperRef) = 0;
  virtual std::shared_ptr<IStep> GetCompleteTaskStep(
      ICompleteTaskStep::TaskWrappers) = 0;
  virtual std::shared_ptr<IStep> GetDeleteTaskStep(
      IDeleteTaskStep::TaskWrappers) = 0;

  virtual std::shared_ptr<IStep> GetShowAllTasksStep(
      IShowAllTasksStep::TaskStorageRef) = 0;
  virtual std::shared_ptr<IStep> GetShowNTasksStep(
      IShowNTasksStep::TaskWrappers) = 0;
  virtual std::shared_ptr<IStep> GetShowSortedTasksStep(
      IShowSortedTasksStep::Tasks) = 0;
  virtual std::shared_ptr<IStep> GetShowHelpStep() = 0;
  virtual std::shared_ptr<IStep> GetReportMessageStep(MessageEnum, TaskId) = 0;
  virtual std::shared_ptr<IStep> GetReportMessageStep(MessageEnum) = 0;

  virtual ~IStepFactory() {}
};

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEPFACTORY_H_
