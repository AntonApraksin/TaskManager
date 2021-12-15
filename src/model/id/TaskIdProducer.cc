#include "TaskIdProducer.h"

TaskIdProducer::TaskIdProducer() : current_id_{0} {}

task_manager::TaskId TaskIdProducer::GetNextId() {
  return CreateTaskId(current_id_++);
}
