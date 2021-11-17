#ifndef TASKMANAGER_SRC_MODEL_TASKIDPRODUCER_H_
#define TASKMANAGER_SRC_MODEL_TASKIDPRODUCER_H_
#pragma once

#include "model/ITaskIdProducer.h"
#include "model/TaskId.h"

class TaskIdProducer final : public ITaskIdProducer {
 public:
  TaskIdProducer();

  TaskId GetNextId() override;

 private:
  int current_id_;
};

#endif  // TASKMANAGER_SRC_MODEL_TASKIDPRODUCER_H_
