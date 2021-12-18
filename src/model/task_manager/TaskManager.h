#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGER_TASKMANAGER_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGER_TASKMANAGER_H_

#include <map>
#include <memory>

#include "model/OperationResult.h"
#include "model/id/TaskIdProducer.h"
#include "model/task/Task.h"

class TaskManager final {
 public:
  using Tasks = std::unordered_map<TaskId, Task>;
  using Parents = std::unordered_map<TaskId, std::vector<TaskId>>;
  using Roots = std::vector<TaskId>;

  struct Storage {
    Tasks tasks;
    Parents parents;
    Roots roots;
  };

  enum class Status {
    kOk,
    kNotPresentId,
  };

  explicit TaskManager(std::unique_ptr<ITaskIdProducer> id_producer);

  TaskManager(std::unique_ptr<ITaskIdProducer> id_producer, Storage);

  OperationResult<Status, TaskId> Add(Task task);
  OperationResult<Status, TaskId> Add(TaskId task_id, Task task);

  OperationResult<Status> Edit(TaskId id, Task task);
  OperationResult<Status> Complete(TaskId id);
  OperationResult<Status> Delete(TaskId id);

  Storage Show() const;

 private:
  Storage storage_;
  std::unique_ptr<ITaskIdProducer> id_producer_;
};

void GetCompleteSubTree(TaskId id, const TaskManager::Parents&,
                        std::vector<TaskId>&);

#endif  // TASKMANAGER_SRC_MODEL_TASKMANAGER_TASKMANAGER_H_
