#ifndef TASKMANAGER_SRC_MODEL_ID_TASKIDPRODUCER_H_
#define TASKMANAGER_SRC_MODEL_ID_TASKIDPRODUCER_H_
#pragma once

#include "ITaskIdProducer.h"
#include "TaskId.h"

class TaskIdProducer final : public ITaskIdProducer {
 public:
  TaskIdProducer();
  explicit TaskIdProducer(TaskId);

  task_manager::TaskId GetNextId() override;
  task_manager::TaskId GetCurrentId() override;

 private:
  google::protobuf::int32 current_id_;
};

#endif  // TASKMANAGER_SRC_MODEL_ID_TASKIDPRODUCER_H_
