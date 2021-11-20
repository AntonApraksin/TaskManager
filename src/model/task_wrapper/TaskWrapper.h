#ifndef TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKWRAPPER_H_
#define TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKWRAPPER_H_

#include <map>

#include "model/id/TaskId.h"
#include "model/task/Task.h"

class TaskWrapper {
 public:
  using Storage = std::map<TaskId, TaskWrapper>;

 public:
  TaskWrapper(Task task);

  void Add(TaskId task_id, Task task);
  void Complete();
  bool Delete(TaskId task_id);

  void SetTask(Task task);
  const Task& GetTask() const;
  const Task& operator*() const;

  TaskWrapper* Find(TaskId task_id);

  const Storage& Show() const;

 private:
  Task task_;
  std::map<TaskId, TaskWrapper> children_;

 private:
  friend class TaskItem;
};

#endif  // TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKWRAPPER_H_
