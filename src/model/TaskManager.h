#ifndef TASK_MANAGER_INCLUDE_MODEL_TASK_MANAGER_H_
#define TASK_MANAGER_INCLUDE_MODEL_TASK_MANAGER_H_
#pragma once

#include <map>
#include <memory>

#include "model/Task.h"
#include "model/TaskIdProducer.h"

class TaskManager final {
 public:
  explicit TaskManager(std::unique_ptr<ITaskIdProducer> id_producer);

  TaskId Add(Task task);

  TaskManager& Edit(TaskId id, Task task);
  TaskManager& Complete(TaskId id);
  TaskManager& Delete(TaskId id);

  const std::map<TaskId, Task>& Show() &;
  std::map<TaskId, Task> Show() &&;

 private:
  std::map<TaskId, Task> tasks_;
  std::unique_ptr<ITaskIdProducer> id_producer_;
};

#endif  // TASK_MANAGER_INCLUDE_MODEL_TASK_MANAGER_H_
