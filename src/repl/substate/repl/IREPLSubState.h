#ifndef TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_
#define TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_

#include "repl/print/IPrinter.h"
#include "repl/print/IValidator.h"
#include "repl/substate/ISubState.h"

class Context;

class IREPLSubState : public ISubState {
 public:
  IREPLSubState(const std::shared_ptr<IPrinter>& printer,
                const std::shared_ptr<IValidator>& validator)
      : printer_(printer), validator_(validator) {}

 protected:
  std::shared_ptr<IPrinter> printer_;
  std::shared_ptr<IValidator> validator_;
};

class ReadTitleREPLSubState : public IREPLSubState {
 public:
  using IREPLSubState::IREPLSubState;
  void Execute(SubContext&) override;
};

class ReadDateREPLSubState : public IREPLSubState {
 public:
  using IREPLSubState::IREPLSubState;
  void Execute(SubContext&) override;
};

class ReadPriorityREPLSubState : public IREPLSubState {
 public:
  using IREPLSubState::IREPLSubState;
  void Execute(SubContext&) override;
};

class ReadIdREPLSubState : public IREPLSubState {
 public:
  using IREPLSubState::IREPLSubState;
  void Execute(SubContext&) override;
};

class ReadConfirmationREPLSubState : public IREPLSubState {
 public:
  using IREPLSubState::IREPLSubState;
  void Execute(SubContext&) override;
};

#endif  // TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_