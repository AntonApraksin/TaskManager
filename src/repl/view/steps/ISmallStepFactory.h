#ifndef TASKMANAGER_SRC_REPL_STATE_FACTORY_ISTATEFACTORY_H_
#define TASKMANAGER_SRC_REPL_STATE_FACTORY_ISTATEFACTORY_H_

#include "repl/validator/IValidator.h"
#include "repl/view/steps/ISmallStep.h"

namespace task_manager {
class ISmallStepFactory {
 public:
  virtual std::shared_ptr<ISmallStep> GetReadTitleSmallStep() = 0;
  virtual std::shared_ptr<ISmallStep> GetReadDateSmallStep() = 0;
  virtual std::shared_ptr<ISmallStep> GetReadPrioritySmallStep() = 0;
  virtual std::shared_ptr<ISmallStep> GetReadStateSmallStep() = 0;

  virtual ~ISmallStepFactory() {}
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_FACTORY_ISTATEFACTORY_H_
