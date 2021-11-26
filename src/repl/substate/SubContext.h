#ifndef TASKMANAGER_SRC_REPL_SUBSTATE_SUBCONTEXT_H_
#define TASKMANAGER_SRC_REPL_SUBSTATE_SUBCONTEXT_H_

#include <deque>

#include "model/id/TaskId.h"
#include "repl/TaskBuilder.h"
#include "repl/substate/ISubState.h"

class SubContext {
 public:
  void PushState(const std::shared_ptr<ISubState>&);
  void PopState();
  void ClearStates();

  TaskBuilder& GetTaskBuilder();
  TaskId& GetTaskId();

  void Run();

 private:
  std::deque<std::shared_ptr<ISubState>> states_;

  TaskBuilder task_builder_;
  TaskId task_id_ = TaskId::Create(0);
};

#endif  // TASKMANAGER_SRC_REPL_SUBSTATE_SUBCONTEXT_H_
