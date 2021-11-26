#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_STATEMACHINE_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_STATEMACHINE_H_

#include "repl/state_factory/IStateFactory.h"

class StateMachine {
 public:
  StateMachine(const std::shared_ptr<IStateFactory>& state_factory,
               const std::shared_ptr<Context>& context)
      : state_factory_(state_factory), context_(context) {}

  void Run() {}

 private:
  std::shared_ptr<IStateFactory> state_factory_;
  std::shared_ptr<Context> context_;
};

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_STATEMACHINE_H_
