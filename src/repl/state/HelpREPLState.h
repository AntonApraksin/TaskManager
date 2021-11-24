#ifndef TASKMANAGER_SRC_REPL_STATE_HELPREPLSTATE_H_
#define TASKMANAGER_SRC_REPL_STATE_HELPREPLSTATE_H_

#include "repl/print/IPrinter.h"
#include "repl/state/IREPLState.h"

class HelpREPLState : public IREPLState {
 public:
  HelpREPLState(const std::shared_ptr<IPrinter>& printer);

  std::shared_ptr<IREPLState> Execute(Context&) override;

 private:
  std::shared_ptr<IPrinter> printer_;
};

#endif  // TASKMANAGER_SRC_REPL_STATE_HELPREPLSTATE_H_
