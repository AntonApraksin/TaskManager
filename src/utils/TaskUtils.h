#ifndef TASKMANAGER_SRC_UTILS_TASKUTILS_H_
#define TASKMANAGER_SRC_UTILS_TASKUTILS_H_

#include <google/protobuf/timestamp.pb.h>

#include "Task.pb.h"

namespace task_manager {
std::optional<Task> CreateTask(std::string, google::protobuf::Timestamp,
                               Task::Priority, Task::Progress);

using TaskDate_t = google::protobuf::Timestamp;
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_UTILS_TASKUTILS_H_
