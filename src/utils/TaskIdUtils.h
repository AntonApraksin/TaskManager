#ifndef TASKMANAGER_SRC_UTILS_TASKIDUTILS_H_
#define TASKMANAGER_SRC_UTILS_TASKIDUTILS_H_

#include "TaskId.pb.h"

namespace task_manager
{
bool operator==(const TaskId&, const TaskId&);
bool operator!=(const TaskId&, const TaskId&);
bool operator<(const TaskId&, const TaskId&);
bool operator>(const TaskId&, const TaskId&);
bool operator<=(const TaskId&, const TaskId&);
bool operator>=(const TaskId&, const TaskId&);

TaskId CreateTaskId(google::protobuf::int32);
}  // namespace task_manager

namespace std
{
template<>
struct hash<task_manager::TaskId>
{
    size_t operator()(const task_manager::TaskId& x) const
    {
        return hash<google::protobuf::int32>()(x.id());
    }
};
}  // namespace std

#endif  // TASKMANAGER_SRC_UTILS_TASKIDUTILS_H_
