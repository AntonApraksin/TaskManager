#ifndef TASKMANAGER_SRC_MODEL_MODELCONTROLLER_H_
#define TASKMANAGER_SRC_MODEL_MODELCONTROLLER_H_

#include <istream>
#include <ostream>

#include "OperationResult.h"
#include "model/SolidTask.h"
#include "persistence/Persistence.h"

namespace task_manager {
class TaskManager;
class FilePersistence;

class ModelController {
 public:
  enum class Status {
    kOk,
    kNotPresentId,
    kLoadFailure,
    kSaveFailure,
    kNotPresentLabel
  };

 public:
  virtual OperationResult<Status, TaskId> Add(Task task) = 0;
  virtual OperationResult<Status, TaskId> Add(TaskId task_id, Task task) = 0;
  virtual OperationResult<Status> Edit(TaskId id, Task task) = 0;
  virtual OperationResult<Status> Complete(TaskId id) = 0;
  virtual OperationResult<Status> Delete(TaskId id) = 0;
  virtual OperationResult<Status, SolidTasks> GetAllSolidTasks() = 0;
  virtual OperationResult<Status, SolidTasks> GetSpecificSolidTasks(
      std::vector<TaskId>) = 0;
  virtual OperationResult<Status> LoadFrom(Persistence&) = 0;
  virtual OperationResult<Status> SaveTo(Persistence&) = 0;

  virtual OperationResult<Status> AddLabel(TaskId task_id, Label label) = 0;
  virtual OperationResult<Status> DeleteLabel(TaskId task_id, Label label) = 0;

  virtual ~ModelController() {}
};

std::optional<std::pair<TaskId, TaskId>> HasParentChildRelationship(
    const SolidTasks& tasks, const std::vector<TaskId>& ids);
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_MODEL_MODELCONTROLLER_H_
