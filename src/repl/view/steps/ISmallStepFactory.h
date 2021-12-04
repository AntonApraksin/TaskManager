#ifndef TASKMANAGER_SRC_REPL_STATE_FACTORY_ISTATEFACTORY_H_
#define TASKMANAGER_SRC_REPL_STATE_FACTORY_ISTATEFACTORY_H_

#include "repl/validator/IValidator.h"
#include "repl/view/steps/iostream/small_step/IostreamSmallStep.h"
#include "repl/view/steps/iostream/small_step/IostreamSmallStepEnum.h"

class ISmallStepFactory {
 public:
  virtual std::shared_ptr<IostreamSmallStep> GetREPLState(
      IostreamSmallStepEnum se) = 0;

  virtual ~ISmallStepFactory() {}
};

#endif  // TASKMANAGER_SRC_REPL_STATE_FACTORY_ISTATEFACTORY_H_
