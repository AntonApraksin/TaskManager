#ifndef TASKMANAGER_SRC_UTILS_FUNCTIONS_H_
#define TASKMANAGER_SRC_UTILS_FUNCTIONS_H_

#include "model/SolidTask.h"

namespace task_manager {
SolidTasks GetTreeFromVector(const SolidTasks& solid_tasks, const TaskId& id);

std::optional<SolidTask> FindSolidTaskById(const SolidTasks& solid_tasks,
                                           const TaskId& task_id);
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_UTILS_FUNCTIONS_H_
