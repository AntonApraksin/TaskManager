#ifndef TASKMANAGER_SRC_REPL_STATE_ADDNEWREPLSTATE_H_
#define TASKMANAGER_SRC_REPL_STATE_ADDNEWREPLSTATE_H_

#include "repl/print/IPrinter.h"
#include "repl/print/IValidator.h"
#include "repl/state/IREPLState.h"

class Context;

class AddNewREPLState : public IREPLState {
 public:
  AddNewREPLState(const std::shared_ptr<IPrinter>& printer,
                  const std::shared_ptr<IValidator>& validator);

  std::shared_ptr<IREPLState> Execute(Context&) override;

 private:
  std::shared_ptr<IPrinter> printer_;
  std::shared_ptr<IValidator> validator_;
};

#endif  // TASKMANAGER_SRC_REPL_STATE_ADDNEWREPLSTATE_H_
