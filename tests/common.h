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

class TaskFactory final {
 public:
  Task GetNextTask() {
    std::stringstream ss;
    ss << "Sample task #" << state_;
    Date_t due_date =
        google::protobuf::util::TimeUtil::TimeTToTimestamp(std::time(nullptr));
    Task::Priority priority = static_cast<Task::Priority>(state_ % 4);
    Task::Progress progress = static_cast<Task::Progress>(state_ % 3);
    ++state_;
    return *CreateTask(ss.str(), due_date, priority, progress);
  }

 private:
  int state_ = 0;
};

#endif  // TASKMANAGER_TESTS_COMMON_H_
