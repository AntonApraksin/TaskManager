#ifndef TASKMANAGER_SRC_MODEL_MODELCONTROLLER_H_
#define TASKMANAGER_SRC_MODEL_MODELCONTROLLER_H_

#include <istream>
#include <ostream>

#include "OperationResult.h"
#include "model/SolidTask.h"

namespace task_manager {
class TaskManager;
class Persistence;

class ModelController {
 public:
  enum class Status {
    kOk,
    kNotPresentId,
    kLoadFailure,
    kSaveFailure,
  };

 public:
  ModelController(std::unique_ptr<TaskManager> task_manager,
                  std::unique_ptr<Persistence> persistence);

  OperationResult<Status, TaskId> Add(Task task);
  OperationResult<Status, TaskId> Add(TaskId task_id, Task task);

  OperationResult<Status> Edit(TaskId id, Task task);
  OperationResult<Status> Complete(TaskId id);
  OperationResult<Status> Delete(TaskId id);

  OperationResult<Status, SolidTasks> GetAllSolidTasks();
  OperationResult<Status, SolidTasks> GetSpecificSolidTasks(
      std::vector<TaskId>);

  OperationResult<Status> LoadFrom(std::istream&);
  OperationResult<Status> SaveTo(std::ostream&);

 private:
  std::unique_ptr<TaskManager> task_manager_;
  std::unique_ptr<Persistence> persistence_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_MODEL_MODELCONTROLLER_H_
