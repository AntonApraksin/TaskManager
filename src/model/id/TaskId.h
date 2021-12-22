#ifndef TASKMANAGER_SRC_MODEL_ID_TASKID_H_
#define TASKMANAGER_SRC_MODEL_ID_TASKID_H_

#include "TaskId.pb.h"

namespace task_manager {
bool operator==(const TaskId&, const TaskId&);
bool operator!=(const TaskId&, const TaskId&);
bool operator<(const TaskId&, const TaskId&);
bool operator>(const TaskId&, const TaskId&);
bool operator<=(const TaskId&, const TaskId&);
bool operator>=(const TaskId&, const TaskId&);

TaskId CreateTaskId(google::protobuf::int32);
}  // namespace task_manager

namespace std {
template <>
struct hash<task_manager::TaskId> {
  size_t operator()(const task_manager::TaskId& x) const {
    return hash<google::protobuf::int32>()(x.id());
  }
};
}  // namespace std

using task_manager::CreateTaskId;
using task_manager::TaskId;

#endif  // TASKMANAGER_SRC_MODEL_ID_TASKID_H_
