#include "TaskIdProducer.h"

#include "utils/TaskIdUtils.h"

namespace task_manager {
TaskIdProducer::TaskIdProducer() : current_id_{0} {}

TaskIdProducer::TaskIdProducer(TaskId id) : current_id_{id.id()} {}

task_manager::TaskId TaskIdProducer::GetNextId() {
  return CreateTaskId(current_id_++);
}

task_manager::TaskId TaskIdProducer::GetCurrentId() {
  return CreateTaskId(current_id_);
}

}  // namespace task_manager