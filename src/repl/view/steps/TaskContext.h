#ifndef TASKMANAGER_SRC_REPL_SUBSTATE_TASKCONTEXT_H_
#define TASKMANAGER_SRC_REPL_SUBSTATE_TASKCONTEXT_H_

#include <deque>

#include "ISmallStep.h"
#include "model/id/TaskId.h"
#include "repl/TaskBuilder.h"

class TaskContext {
 public:
  void PushState(const std::shared_ptr<ISmallStep>&);
  void PopState();
  void ClearStates();

  TaskBuilder& GetTaskBuilder();

  void Run();

 private:
  std::deque<std::shared_ptr<ISmallStep>> states_;

  TaskBuilder task_builder_;
};

#endif  // TASKMANAGER_SRC_REPL_SUBSTATE_TASKCONTEXT_H_
