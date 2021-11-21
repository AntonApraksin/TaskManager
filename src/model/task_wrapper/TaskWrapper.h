#ifndef TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKWRAPPER_H_
#define TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKWRAPPER_H_

#include <map>

#include "model/id/TaskId.h"
#include "model/task/Task.h"

class TaskWrapper final {
 public:
  using Storage = std::map<TaskId, TaskWrapper>;

 public:
  const Task& GetTask() const;
  const Task& operator*() const;
  const Storage& Show() const;

  const TaskWrapper& Find(TaskId) const;

 private:
  TaskWrapper(Task task);

  void Add(TaskId task_id, Task task);
  void Complete();
  bool Delete(TaskId task_id);

  void SetTask(Task task);

  TaskWrapper* FindImpl(TaskId task_id);
  const TaskWrapper* FindImpl(TaskId task_id) const;

 private:
  Task task_;
  std::map<TaskId, TaskWrapper> children_;

 private:
  friend class TaskStorage;
};

#endif  // TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKWRAPPER_H_