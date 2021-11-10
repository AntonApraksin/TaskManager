#ifndef TASK_MANAGER_SRC_MODEL_TASK_MANAGER_H_
#define TASK_MANAGER_SRC_MODEL_TASK_MANAGER_H_
#pragma once

#include <map>
#include <stdexcept>

#include "Model/task.h"

class TaskManager {
 public:
  void Add(Task task);

  void Edit(int id, Task task);

  void Complete(int id);

  void Delete(int id);

  std::map<int, Task> Show();

 private:
  std::map<int, Task> tasks_;
};

#endif  // TASK_MANAGER_SRC_MODEL_TASK_MANAGER_H_
