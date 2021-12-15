#ifndef TASKMANAGER_SRC_MODEL_ID_TASKIDPRODUCER_H_
#define TASKMANAGER_SRC_MODEL_ID_TASKIDPRODUCER_H_
#pragma once

#include "ITaskIdProducer.h"
#include "TaskId.h"

class TaskIdProducer final : public ITaskIdProducer {
 public:
  TaskIdProducer();

  task_manager::TaskId GetNextId() override;

 private:
  google::protobuf::int32 current_id_;
};

#endif  // TASKMANAGER_SRC_MODEL_ID_TASKIDPRODUCER_H_
