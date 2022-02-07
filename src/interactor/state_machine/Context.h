#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_CONTEXT_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_CONTEXT_H_

#include <optional>

#include "interactor/state_machine/StepEvent.h"
#include "model/SolidTask.h"

namespace task_manager {
struct Context {
  std::optional<TaskId> task_id;
  std::optional<SolidTasks> solid_tasks;
  StepEvent event;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_CONTEXT_H_
