#include "model/id/TaskId.h"

TaskId task_manager::CreateTaskId(google::protobuf::int32 id) {
  TaskId task_id;
  task_id.set_id(id);
  return task_id;
}

namespace task_manager {
bool operator==(const TaskId& lhs, const TaskId& rhs) {
  return lhs.id() == rhs.id();
}

bool operator!=(const TaskId& lhs, const TaskId& rhs) {
  return lhs.id() != rhs.id();
}

bool operator<(const TaskId& lhs, const TaskId& rhs) {
  return lhs.id() < rhs.id();
}

bool operator>(const TaskId& lhs, const TaskId& rhs) {
  return lhs.id() > rhs.id();
}

bool operator>=(const TaskId& lhs, const TaskId& rhs) {
  return lhs.id() >= rhs.id();
}

bool operator<=(const TaskId& lhs, const TaskId& rhs) {
  return lhs.id() <= rhs.id();
}

}  // namespace task_manager