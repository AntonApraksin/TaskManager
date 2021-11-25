#ifndef TASKMANAGER_SRC_REPL_CONTEXT_CONTEXT_H_
#define TASKMANAGER_SRC_REPL_CONTEXT_CONTEXT_H_

#include <memory>

#include "repl/print/IPrinter.h"
#include "repl/state/IREPLState.h"
#include "repl/state_factory/IStateFactory.h"

class Context {
 public:
  Context(const std::shared_ptr<IREPLState>& state);

  void Run();

 private:
  std::shared_ptr<IREPLState> state_;
};

#endif  // TASKMANAGER_SRC_REPL_CONTEXT_CONTEXT_H_
