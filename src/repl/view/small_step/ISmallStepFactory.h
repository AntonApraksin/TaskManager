#ifndef TASKMANAGER_SRC_REPL_STATE_FACTORY_ISTATEFACTORY_H_
#define TASKMANAGER_SRC_REPL_STATE_FACTORY_ISTATEFACTORY_H_

#include "repl/io_facility/IValidator.h"
#include "repl/view/small_step/iostream_small_step/IostreamSmallStep.h"
#include "repl/view/small_step/iostream_small_step/IostreamSmallStepEnum.h"

class ISmallStepFactory {
 public:
  virtual std::shared_ptr<IostreamSmallStep> GetREPLState(
      IostreamSmallStepEnum se) = 0;

  virtual ~ISmallStepFactory() {}
};

#endif  // TASKMANAGER_SRC_REPL_STATE_FACTORY_ISTATEFACTORY_H_
