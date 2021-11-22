#ifndef TASKMANAGER_SRC_REPL_CONTEXT_CONTEXT_H_
#define TASKMANAGER_SRC_REPL_CONTEXT_CONTEXT_H_

#include "repl/state/IREPLState.h"
#include "repl/print/IPrinter.h"

#include <memory>

class Context {
 public:
  Context(std::shared_ptr<IREPLState> state, std::shared_ptr<IPrinter> printer)
    : state_(std::move(state)), printer_(std::move(printer)) {}

  std::shared_ptr<IPrinter> GetPrinter()
  {
    return printer_;
  }

  void Run()
  {
    for(; state_ != nullptr;)
    {
      state_.reset(state_->Execute(this));
    }
  }

 private:
  std::shared_ptr<IREPLState> state_;
  std::shared_ptr<IPrinter> printer_;
};

#endif //TASKMANAGER_SRC_REPL_CONTEXT_CONTEXT_H_
