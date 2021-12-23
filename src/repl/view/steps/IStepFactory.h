#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEPFACTORY_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEPFACTORY_H_

#include "repl/view/steps/IStep.h"

namespace task_manager {
class IStepFactory {
 public:
  virtual std::shared_ptr<IStep> GetAddTaskStep() = 0;
  virtual std::shared_ptr<IStep> GetAddTaskStep(SolidTask) = 0;

  virtual std::shared_ptr<IStep> GetEditTaskStep(SolidTask) = 0;
  virtual std::shared_ptr<IStep> GetCompleteTaskStep(SolidTasks) = 0;
  virtual std::shared_ptr<IStep> GetDeleteTaskStep(SolidTasks) = 0;

  virtual std::shared_ptr<IStep> GetShowStep(SolidTasks) = 0;
  virtual std::shared_ptr<IStep> GetShowHelpStep() = 0;
  virtual std::shared_ptr<IStep> GetReportMessageStep(std::string) = 0;

  virtual ~IStepFactory() {}
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEPFACTORY_H_
