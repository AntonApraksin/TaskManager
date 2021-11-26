#ifndef TASKMANAGER_SRC_REPL_STATE_FACTORY_ISTATEFACTORY_H_
#define TASKMANAGER_SRC_REPL_STATE_FACTORY_ISTATEFACTORY_H_

#include "repl/commit/CommitStateEnum.h"
#include "repl/commit/ICommitState.h"
#include "repl/print/IValidator.h"
#include "repl/state/IREPLState.h"
#include "repl/state/StateEnum.h"
#include "repl/substate/IREPLSubState.h"
#include "repl/substate/SubStateEnum.h"

class IStateFactory {
 public:
  virtual std::shared_ptr<IREPLState> GetState(StateEnum se) = 0;
  virtual std::shared_ptr<IREPLSubState> GetSubState(SubStateEnum se) = 0;
  virtual std::shared_ptr<ICommitState> GetCommitState(CommitStateEnum se) = 0;

  virtual ~IStateFactory() {}
};

#endif  // TASKMANAGER_SRC_REPL_STATE_FACTORY_ISTATEFACTORY_H_
