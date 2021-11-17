#ifndef TASK_MANAGER_INCLUDE_MODEL_TASK_ID_PRODUCER_H_
#define TASK_MANAGER_INCLUDE_MODEL_TASK_ID_PRODUCER_H_
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

#endif  // TASK_MANAGER_INCLUDE_MODEL_TASK_ID_PRODUCER_H_
