#ifndef TASKMANAGER_SRC_REPL_STATE_IREPLSTATE_H_
#define TASKMANAGER_SRC_REPL_STATE_IREPLSTATE_H_

#include <memory>

#include "repl/print/IPrinter.h"
#include "repl/print/IValidator.h"

class Context;

class IREPLState {
 public:
  virtual StateEnum Execute(const std::shared_ptr<Context>&) = 0;
  virtual ~IREPLState() {}
};

class IPrinterREPLState : public IREPLState {
 public:
  IPrinterREPLState(const std::shared_ptr<IPrinter>& printer)
      : printer_(printer) {}

 protected:
  std::shared_ptr<IPrinter> printer_;
};

class IValidatorREPLState : public IREPLState {
 public:
  IValidatorREPLState(const std::shared_ptr<IValidator>& validator)
      : validator_(validator) {}

 protected:
  std::shared_ptr<IValidator> validator_;
};

class IPrinterValidatorREPLState : public IREPLState {
 public:
  IPrinterValidatorREPLState(const std::shared_ptr<IPrinter>& printer,
                             const std::shared_ptr<IValidator>& validator)
      : printer_(printer), validator_(validator) {}

 protected:
  std::shared_ptr<IPrinter> printer_;
  std::shared_ptr<IValidator> validator_;
};

class AddNewREPLState : public IPrinterValidatorREPLState {
 public:
  using IPrinterValidatorREPLState::IPrinterValidatorREPLState;

  StateEnum Execute(const std::shared_ptr<Context>&) override;
};

class HelpREPLState : public IPrinterREPLState {
 public:
  using IPrinterREPLState::IPrinterREPLState;

  StateEnum Execute(const std::shared_ptr<Context>&) override;
};

class MainREPLState : public IPrinterValidatorREPLState {
 public:
  using IPrinterValidatorREPLState::IPrinterValidatorREPLState;

  StateEnum Execute(const std::shared_ptr<Context>&) override;
};

class UnknownREPLState : public IPrinterREPLState {
 public:
  using IPrinterREPLState::IPrinterREPLState;

  StateEnum Execute(const std::shared_ptr<Context>&) override;
};

#endif  // TASKMANAGER_SRC_REPL_STATE_IREPLSTATE_H_