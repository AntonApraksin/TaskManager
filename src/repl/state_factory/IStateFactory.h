#ifndef TASKMANAGER_SRC_REPL_STATE_FACTORY_ISTATEFACTORY_H_
#define TASKMANAGER_SRC_REPL_STATE_FACTORY_ISTATEFACTORY_H_

#include "repl/print/IValidator.h"
#include "repl/state/IREPLState.h"

class IStateFactory {
 public:
  virtual std::shared_ptr<IREPLState> GetState(StateEnum se) = 0;

  virtual ~IStateFactory() {}
};

#endif  // TASKMANAGER_SRC_REPL_STATE_FACTORY_ISTATEFACTORY_H_
