#include "TaskUtils.h"

namespace task_manager {
std::optional<Task> CreateTask(std::string title,
                               google::protobuf::Timestamp due_date,
                               Task::Priority priority,
                               Task::Progress progress) {
  if (title.empty()) {
    return {};
  }
  Task task;
  task.set_title(std::move(title));
  task.set_allocated_due_date(
      new google::protobuf::Timestamp(std::move(due_date)));
  task.set_priority(priority);
  task.set_progress(progress);
  return task;
}
}  // namespace task_manager
