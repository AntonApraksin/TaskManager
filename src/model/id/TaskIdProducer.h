#ifndef TASKMANAGER_SRC_MODEL_ID_TASKIDPRODUCER_H_
#define TASKMANAGER_SRC_MODEL_ID_TASKIDPRODUCER_H_

#include "ITaskIdProducer.h"
#include "TaskId.pb.h"

namespace task_manager {
class TaskIdProducer final : public ITaskIdProducer {
 public:
  TaskIdProducer();
  explicit TaskIdProducer(TaskId);

  task_manager::TaskId GetNextId() override;
  task_manager::TaskId GetCurrentId() override;

 private:
  google::protobuf::int32 current_id_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_MODEL_ID_TASKIDPRODUCER_H_
