#ifndef TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKSTORAGE_H_
#define TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKSTORAGE_H_

#include <map>

#include "model/id/TaskId.h"
#include "model/task_wrapper/TaskWrapper.h"

class TaskStorage {
 public:
  using Storage = std::map<TaskId, TaskWrapper>;

 public:
  TaskStorage();
  TaskStorage(const TaskStorage&) = delete;
  TaskStorage(TaskStorage&&) = delete;

  TaskStorage& operator=(const TaskStorage&) = delete;
  TaskStorage& operator=(TaskStorage&&) = delete;

  void Add(TaskId task_id, Task task);
  void Add(TaskId parent, TaskId task_id, Task task);
  void Complete(TaskId task_id);
  void Delete(TaskId task_id);
  void Edit(TaskId task_id, Task task);

  const Storage& Show() const;

 private:
  TaskWrapper* Find(TaskId task_id);

 private:
  size_t size_ = 0;
  Storage children_;
};

#endif  // TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKSTORAGE_H_
