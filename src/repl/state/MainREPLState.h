#ifndef TASKMANAGER_SRC_REPL_STATE_MAINREPLSTATE_H_
#define TASKMANAGER_SRC_REPL_STATE_MAINREPLSTATE_H_

#include "repl/state/IREPLState.h"

class Context;

class MainREPLState : public IREPLState {
 public:
  IREPLState* Execute(Context*) override;
};

#endif //TASKMANAGER_SRC_REPL_STATE_MAINREPLSTATE_H_
