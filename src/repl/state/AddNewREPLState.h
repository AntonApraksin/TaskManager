#ifndef TASKMANAGER_SRC_REPL_STATE_ADDNEWREPLSTATE_H_
#define TASKMANAGER_SRC_REPL_STATE_ADDNEWREPLSTATE_H_

#include "repl/state/IREPLState.h"

class Context;

class AddNewREPLState : public IREPLState {
 public:
  IREPLState* Execute(Context*) override;
};

#endif //TASKMANAGER_SRC_REPL_STATE_ADDNEWREPLSTATE_H_
