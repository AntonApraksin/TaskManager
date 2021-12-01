#ifndef TASKMANAGER_SRC_REPL_SUBSTATE_TASKCONTEXT_H_
#define TASKMANAGER_SRC_REPL_SUBSTATE_TASKCONTEXT_H_

#include <deque>

#include "model/id/TaskId.h"
#include "repl/TaskBuilder.h"
#include "repl/substate/ISubState.h"

class TaskContext {
 public:
  void PushState(const std::shared_ptr<ISubState>&);
  void PopState();
  void ClearStates();

  TaskBuilder& GetTaskBuilder();

  void Run();

 private:
  std::deque<std::shared_ptr<ISubState>> states_;

  TaskBuilder task_builder_;
};

#endif  // TASKMANAGER_SRC_REPL_SUBSTATE_TASKCONTEXT_H_
