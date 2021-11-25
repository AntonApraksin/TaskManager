#ifndef TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_
#define TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_

#include "repl/print/IPrinter.h"
#include "repl/print/IValidator.h"

class Context;

class IREPLSubState {
 public:
  IREPLSubState(const std::shared_ptr<IPrinter>& printer,
                const std::shared_ptr<IValidator>& validator)
      : printer_(printer), validator_(validator) {}

  virtual std::shared_ptr<IREPLSubState> Execute(Context&) = 0;

  virtual ~IREPLSubState() {}

 protected:
  std::shared_ptr<IPrinter> printer_;
  std::shared_ptr<IValidator> validator_;
};

class ReadTitleREPLSubState : public IREPLSubState {
 public:
  using IREPLSubState::IREPLSubState;
  std::shared_ptr<IREPLSubState> Execute(Context&) override;
};

class ReadDateREPLSubState : public IREPLSubState {
 public:
  using IREPLSubState::IREPLSubState;
  std::shared_ptr<IREPLSubState> Execute(Context&) override;
};

class ReadPriorityREPLSubState : public IREPLSubState {
 public:
  using IREPLSubState::IREPLSubState;
  std::shared_ptr<IREPLSubState> Execute(Context&) override;
};

class ReadIdREPLSubState : public IREPLSubState {
 public:
  using IREPLSubState::IREPLSubState;
  std::shared_ptr<IREPLSubState> Execute(Context&) override;
};

class ReadConfirmationREPLSubState : public IREPLSubState {
 public:
  using IREPLSubState::IREPLSubState;
  std::shared_ptr<IREPLSubState> Execute(Context&) override;
};

#endif  // TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_
