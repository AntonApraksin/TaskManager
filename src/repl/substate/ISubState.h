#ifndef TASKMANAGER_SRC_REPL_SUBSTATE_ISUBSTATE_H_
#define TASKMANAGER_SRC_REPL_SUBSTATE_ISUBSTATE_H_

class SubContext;

class ISubState {
 public:
  virtual ~ISubState() {}
  virtual void Execute(SubContext& ctx) = 0;
};

#endif  // TASKMANAGER_SRC_REPL_SUBSTATE_ISUBSTATE_H_
