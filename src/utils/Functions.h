#ifndef TASKMANAGER_SRC_UTILS_FUNCTIONS_H_
#define TASKMANAGER_SRC_UTILS_FUNCTIONS_H_

#include "model/SolidTask.h"

namespace task_manager {
std::pair<SolidTasks::iterator, SolidTasks::iterator> GetTreeFromVector(
    SolidTasks& solid_tasks, TaskId id);
}

#endif  // TASKMANAGER_SRC_UTILS_FUNCTIONS_H_
