#ifndef TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKWRAPPER_H_
#define TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKWRAPPER_H_

#include <map>

#include "model/id/TaskId.h"
#include "model/task/Task.h"
#include "model/task_wrapper/TaskStorage.h"

class TaskWrapper : public _TaskStorageImpl<TaskWrapper> {
 public:
  explicit TaskWrapper(Task task);

  const Task& GetTask() const;
  const Task& operator*() const;

  const Task* operator->() const;

  void Complete();
  void SetTask(Task task);

 private:
  Task task_;
};

using TaskStorage = _TaskStorageImpl<TaskWrapper>;

#endif  // TASKMANAGER_SRC_MODEL_TASK_WRAPPER_TASKWRAPPER_H_