#ifndef TASKMANAGER_SRC_MODEL_ID_ITASKIDPRODUCER_H_
#define TASKMANAGER_SRC_MODEL_ID_ITASKIDPRODUCER_H_
#pragma once

#include "TaskId.h"

class ITaskIdProducer {
 public:
  virtual TaskId GetNextId() = 0;

  virtual ~ITaskIdProducer(){};
};

#endif  // TASKMANAGER_SRC_MODEL_ID_ITASKIDPRODUCER_H_