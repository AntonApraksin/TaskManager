#ifndef TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKSTORAGE_H_
#define TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKSTORAGE_H_

#include <map>

#include "model/id/TaskId.h"
#include "model/task_wrapper/TaskWrapper.h"

class TaskStorage final {
 public:
  using Storage = std::map<TaskId, TaskWrapper>;

 public:
  void Add(TaskId task_id, Task task);
  void Add(TaskId parent, TaskId task_id, Task task);
  void Complete(TaskId task_id);
  void Delete(TaskId task_id);
  void Edit(TaskId task_id, Task task);

  const Storage& Show() const;
  const TaskWrapper& Find(TaskId) const;

 private:
  TaskWrapper* FindImpl(TaskId task_id);
  const TaskWrapper* FindImpl(TaskId task_id) const;

 private:
  Storage children_;
};

#endif  // TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKSTORAGE_H_
