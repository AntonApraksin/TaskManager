#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGER_TASKMANAGER_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGER_TASKMANAGER_H_

#include <map>
#include <memory>

#include "OperationResult.h"
#include "Task.pb.h"
#include "model/id/TaskIdProducer.h"
#include "utils/TaskIdUtils.h"

namespace task_manager {
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

  enum class Status { kOk, kNotPresentId, kNotPresentLabel };

  explicit TaskManager(std::unique_ptr<ITaskIdProducer> id_producer);

  TaskManager(std::unique_ptr<ITaskIdProducer> id_producer, Storage);

  OperationResult<Status, TaskId> Add(Task task);
  OperationResult<Status, TaskId> Add(TaskId task_id, Task task);

  OperationResult<Status> Edit(TaskId id, Task task);
  OperationResult<Status> Complete(TaskId id);
  OperationResult<Status> Delete(TaskId id);

  OperationResult<Status> AddLabel(TaskId id, Label label);
  OperationResult<Status> DeleteLabel(TaskId id, Label label);

  OperationResult<Status, Storage> Show() const;

 private:
  Storage storage_;
  std::unique_ptr<ITaskIdProducer> id_producer_;
};

void GetCompleteSubTree(TaskId id, const TaskManager::Parents&,
                        std::vector<TaskId>&);
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_MODEL_TASKMANAGER_TASKMANAGER_H_
