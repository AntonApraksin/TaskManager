#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGER_TASKMANAGER_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGER_TASKMANAGER_H_
#pragma once

#include <map>
#include <memory>

#include "model/id/TaskIdProducer.h"
#include "model/task/Task.h"
#include "model/task_wrapper/TaskStorage.h"

class TaskManager final {
 public:
  explicit TaskManager(std::unique_ptr<ITaskIdProducer> id_producer);

  TaskId Add(Task task);

  TaskManager& Edit(TaskId id, Task task);
  TaskManager& Complete(TaskId id);
  TaskManager& Delete(TaskId id);

  const TaskStorage::Storage& Show() const;

 private:
  TaskStorage storage_;
  std::unique_ptr<ITaskIdProducer> id_producer_;
};

#endif  // TASKMANAGER_SRC_MODEL_TASKMANAGER_TASKMANAGER_H_
