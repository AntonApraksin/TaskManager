#ifndef TASK_MANAGER_INCLUDE_MODEL_TASK_MANAGER_H_
#define TASK_MANAGER_INCLUDE_MODEL_TASK_MANAGER_H_
#pragma once

#include <map>
#include <stdexcept>

#include "Model/id.h"
#include "Model/task.h"

class TaskManager {
 public:
  void Add(Task task);

  void Edit(TaskId id, Task task);

  void Complete(TaskId id);

  void Delete(TaskId id);

  const std::map<TaskId, Task>& Show();

 private:
  std::map<TaskId, Task> tasks_;
  TaskIdProducer id_producer_;
};

#endif  // TASK_MANAGER_INCLUDE_MODEL_TASK_MANAGER_H_
