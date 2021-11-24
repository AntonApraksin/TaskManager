#ifndef TASKMANAGER_SRC_REPL_STATE_IREPLSTATE_H_
#define TASKMANAGER_SRC_REPL_STATE_IREPLSTATE_H_

#include <memory>

class Context;

class IREPLState {
 public:
  virtual std::shared_ptr<IREPLState> Execute(Context&) = 0;
  virtual ~IREPLState() {}
};

#endif  // TASKMANAGER_SRC_REPL_STATE_IREPLSTATE_H_