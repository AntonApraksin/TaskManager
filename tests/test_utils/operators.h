#ifndef TASKMANAGER_TESTS_TEST_UTILS_OPERATORS_H_
#define TASKMANAGER_TESTS_TEST_UTILS_OPERATORS_H_

#include <google/protobuf/util/time_util.h>

#include "SolidTask.pb.h"
#include "Task.pb.h"
#include "TaskId.pb.h"
#include "test_utils/common.h"
#include "utils/TaskIdUtils.h"

namespace task_manager {
inline bool operator!=(const TaskId& lhs, const TaskId& rhs) {
  return !(lhs == rhs);
}

inline bool operator==(const Task& lhs, const Task& rhs) {
  return lhs.title() == rhs.title() && lhs.priority() == rhs.priority() &&
         lhs.progress() == rhs.progress() && lhs.due_date() == rhs.due_date();
}

inline bool operator==(const SolidTask& lhs, const SolidTask& rhs) {
  if ((lhs.has_parent_id() && !rhs.has_parent_id()) or
      (!lhs.has_parent_id() && rhs.has_parent_id())) {
    return false;
  }
  return lhs.task() == rhs.task() && lhs.task_id() == rhs.task_id() &&
         lhs.parent_id() == rhs.parent_id();
}
}  // namespace task_manager

#endif  // TASKMANAGER_TESTS_TEST_UTILS_OPERATORS_H_
