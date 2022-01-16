#ifndef TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_COMMANDS_COMMANDRESULT_H_
#define TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_COMMANDS_COMMANDRESULT_H_

#include <optional>

#include "model/ModelController.h"
#include "model/SolidTask.h"

namespace task_manager {
struct CommandResult {
  std::optional<TaskId> task_id;
  std::optional<SolidTasks> solid_tasks;
  ModelController::Status status;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_COMMANDS_COMMANDRESULT_H_
