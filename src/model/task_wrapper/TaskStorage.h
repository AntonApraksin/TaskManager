#ifndef TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKSTORAGE_H_
#define TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKSTORAGE_H_

#include <map>

#include "model/id/TaskId.h"
#include "model/task/Task.h"

template<typename T>
class _TaskStorageImpl {
 public:
  using Storage = std::map<TaskId, T>;

 public:
  void Add(TaskId task_id, Task task);
  void Delete(TaskId task_id);

  const Storage& ShowStorage() const;
  const T& Find(TaskId) const;
  T& Find(TaskId);

  const _TaskStorageImpl& FindParentOf(TaskId) const;
  _TaskStorageImpl& FindParentOf(TaskId);

 private:
  _TaskStorageImpl* FindParentOfImpl(TaskId task_id);
  const _TaskStorageImpl* FindParentOfImpl(TaskId task_id) const;

 protected:
  Storage children_;
};

#include "TaskStorage.inl"

#endif  // TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKSTORAGE_H_