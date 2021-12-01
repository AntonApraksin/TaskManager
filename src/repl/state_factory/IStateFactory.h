#ifndef TASKMANAGER_SRC_REPL_STATE_FACTORY_ISTATEFACTORY_H_
#define TASKMANAGER_SRC_REPL_STATE_FACTORY_ISTATEFACTORY_H_

#include "repl/io_facility/IValidator.h"
#include "repl/substate/repl/IREPLSubState.h"
#include "repl/substate/repl/SubStateEnum.h"

class IStateFactory {
 public:
  virtual std::shared_ptr<IREPLSubState> GetREPLState(SubStateEnum se) = 0;

  virtual ~IStateFactory() {}
};

#endif  // TASKMANAGER_SRC_REPL_STATE_FACTORY_ISTATEFACTORY_H_
