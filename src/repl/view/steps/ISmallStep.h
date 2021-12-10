#ifndef TASKMANAGER_SRC_REPL_SUBSTATE_ISUBSTATE_H_
#define TASKMANAGER_SRC_REPL_SUBSTATE_ISUBSTATE_H_

class TaskContext;

class ISmallStep {
 public:
  virtual ~ISmallStep() {}
  virtual void Execute(TaskContext& ctx) = 0;
};

#endif  // TASKMANAGER_SRC_REPL_SUBSTATE_ISUBSTATE_H_
