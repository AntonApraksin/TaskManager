#ifndef TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_
#define TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_

#include "repl/io_facility/IIOFacility.h"
#include "repl/io_facility/IValidator.h"
#include "repl/substate/ISubState.h"

class Context;

class IREPLSubState : public ISubState {
 public:
  IREPLSubState(const std::shared_ptr<IIOFacility>& printer,
                const std::shared_ptr<IValidator>& validator)
      : printer_(printer), validator_(validator) {}

 protected:
  std::shared_ptr<IIOFacility> printer_;
  std::shared_ptr<IValidator> validator_;
};

class ReadTitleREPLSubState : public IREPLSubState {
 public:
  using IREPLSubState::IREPLSubState;
  void Execute(TaskContext&) override;
};

class ReadDateREPLSubState : public IREPLSubState {
 public:
  using IREPLSubState::IREPLSubState;
  void Execute(TaskContext&) override;
};

class ReadPriorityREPLSubState : public IREPLSubState {
 public:
  using IREPLSubState::IREPLSubState;
  void Execute(TaskContext&) override;
};

#endif  // TASKMANAGER_SRC_REPL_STATE_IREPLSUBSTATE_H_