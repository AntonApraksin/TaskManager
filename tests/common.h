#ifndef TASKMANAGER_TESTS_COMMON_H_
#define TASKMANAGER_TESTS_COMMON_H_

#include <google/protobuf/util/time_util.h>

#include <sstream>

#include "Task.pb.h"
#include "TaskId.pb.h"
#include "model/SolidTask.h"
#include "utils/TaskIdUtils.h"
#include "utils/TaskUtils.h"

using namespace task_manager;  // NOLINT: Testing purpose

namespace task_manager {
inline bool operator!=(const TaskId& lhs, const TaskId& rhs) {
  return !(lhs == rhs);
}

inline bool operator==(const Task& lhs, const Task& rhs) {
  return lhs.title() == rhs.title() && lhs.priority() == rhs.priority() &&
         lhs.progress() == rhs.progress() && lhs.due_date() == rhs.due_date();
}

inline bool operator==(const task_manager::SolidTask& lhs,
                       const task_manager::SolidTask& rhs) {
  if ((lhs.has_parent_id() && !rhs.has_parent_id()) or
      (!lhs.has_parent_id() && rhs.has_parent_id())) {
    return false;
  }
  return lhs.task() == rhs.task() && lhs.task_id() == rhs.task_id() &&
         lhs.parent_id() == rhs.parent_id();
}
}  // namespace task_manager

class TaskFactory final {
 public:
  Task GetNextTask() {
    std::stringstream ss;
    ss << "Sample task #" << state_;
    TaskDate_t due_date =
        google::protobuf::util::TimeUtil::TimeTToTimestamp(std::time(nullptr));
    Task::Priority priority = static_cast<Task::Priority>(state_ % 3);
    Task::Progress progress = Task::kUncompleted;
    ++state_;
    return *CreateTask(ss.str(), due_date, priority, progress);
  }

 private:
  int state_ = 0;
};

inline SolidTask FindSolidTask(const SolidTasks& solid_tasks,
                               google::protobuf::int32 id) {
  return *std::find_if(solid_tasks.cbegin(), solid_tasks.cend(),
                       [id](const auto& i) { return i.task_id().id() == id; });
}

inline SolidTask TaskToSolidTask(const Task& task, google::protobuf::int32 id) {
  auto task_id = CreateTaskId(id);
  SolidTask solid_task;
  solid_task.set_allocated_task(new Task(std::move(task)));
  solid_task.set_allocated_task_id(new TaskId(task_id));
  return solid_task;
}

inline SolidTask TaskToSolidTask(const Task& task, google::protobuf::int32 id,
                                 google::protobuf::int32 parent_id) {
  auto solid_task = TaskToSolidTask(task, id);
  auto parent_task_id = CreateTaskId(parent_id);
  solid_task.set_allocated_parent_id(new TaskId(parent_task_id));
  return solid_task;
}

#endif  // TASKMANAGER_TESTS_COMMON_H_
