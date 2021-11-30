#ifndef TASKMANAGER_SRC_REPL_SUBSTATE_ISUBSTATE_H_
#define TASKMANAGER_SRC_REPL_SUBSTATE_ISUBSTATE_H_

class TaskContext;

class ISubState {
 public:
  virtual ~ISubState() {}
  virtual void Execute(TaskContext& ctx) = 0;
};

#endif  // TASKMANAGER_SRC_REPL_SUBSTATE_ISUBSTATE_H_
