#ifndef TASKMANAGER_SRC_MODEL_TASK_TASK_H_
#define TASKMANAGER_SRC_MODEL_TASK_TASK_H_

#include "Task.pb.h"
#include "model/task/TaskDate.h"

namespace task_manager {
std::optional<Task> CreateTask(std::string, google::protobuf::Timestamp,
                               Task::Priority, Task::Progress);
}

using task_manager::CreateTask;
using task_manager::Task;

#endif  // TASKMANAGER_SRC_MODEL_TASK_TASK_H_
