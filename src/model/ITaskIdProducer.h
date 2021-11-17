#ifndef TASK_MANAGER_INCLUDE_MODEL_I_TASK_ID_PRODUCER_H_
#define TASK_MANAGER_INCLUDE_MODEL_I_TASK_ID_PRODUCER_H_
#pragma once

#include "model/TaskId.h"

class ITaskIdProducer {
 public:
  virtual TaskId GetNextId() = 0;

  virtual ~ITaskIdProducer(){};
};

#endif  // TASK_MANAGER_INCLUDE_MODEL_I_TASK_ID_PRODUCER_H_