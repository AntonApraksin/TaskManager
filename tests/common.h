#ifndef TASKMANAGER_TESTS_COMMON_H_
#define TASKMANAGER_TESTS_COMMON_H_

#include <google/protobuf/util/time_util.h>

#include "model/id/TaskId.h"
#include "model/task/Task.h"

namespace task_manager {
inline bool operator!=(const TaskId& lhs, const TaskId& rhs) {
  return !(lhs == rhs);
}

inline bool operator==(const Task& lhs, const Task& rhs) {
  return lhs.title() == rhs.title() && lhs.priority() == rhs.priority() &&
         lhs.progress() == rhs.progress() && lhs.due_date() == rhs.due_date();
}
}  // namespace task_manager

#endif  // TASKMANAGER_TESTS_COMMON_H_
