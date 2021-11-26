#ifndef TASKMANAGER_SRC_REPL_CONTEXT_CONTEXT_H_
#define TASKMANAGER_SRC_REPL_CONTEXT_CONTEXT_H_

#include <memory>

#include "repl/TaskBuilder.h"
#include "repl/print/IPrinter.h"
#include "repl/state/IREPLState.h"
#include "repl/state_factory/IStateFactory.h"
#include "repl/substate/repl/IREPLSubState.h"

class Context {
 public:
  Context(std::unique_ptr<IStateFactory> state_factory);

  void Run();

  IStateFactory& GetStateFactory();

 private:
  std::unique_ptr<IStateFactory> state_factory_;
  std::shared_ptr<IREPLState> state_;
};

#endif  // TASKMANAGER_SRC_REPL_CONTEXT_CONTEXT_H_