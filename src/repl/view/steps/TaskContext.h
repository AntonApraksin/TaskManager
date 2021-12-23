#ifndef TASKMANAGER_SRC_REPL_SUBSTATE_TASKCONTEXT_H_
#define TASKMANAGER_SRC_REPL_SUBSTATE_TASKCONTEXT_H_

#include <deque>

#include "TaskId.pb.h"
#include "repl/view/steps/ISmallStep.h"
#include "utils/TaskBuilder.h"

namespace task_manager {
class TaskContext {
 public:
  void PushState(const std::shared_ptr<ISmallStep>&);
  void PopState();

  TaskBuilder& GetTaskBuilder();

  void Run();

 private:
  std::deque<std::shared_ptr<ISmallStep>> states_;

  TaskBuilder task_builder_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_SUBSTATE_TASKCONTEXT_H_
