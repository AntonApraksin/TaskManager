#ifndef TASKMANAGER_SRC_MODEL_DEFAULTMODELCONTROLLER_H_
#define TASKMANAGER_SRC_MODEL_DEFAULTMODELCONTROLLER_H_

#include <istream>
#include <ostream>

#include "OperationResult.h"
#include "model/ModelController.h"
#include "model/SolidTask.h"

namespace task_manager {
class TaskManager;
class Persistence;

class DefaultModelController : public ModelController {
 public:
  explicit DefaultModelController(std::unique_ptr<TaskManager> task_manager,
                                  std::unique_ptr<Persistence> persistence);

  OperationResult<Status, TaskId> Add(Task task) override;
  OperationResult<Status, TaskId> Add(TaskId task_id, Task task) override;

  OperationResult<Status> Edit(TaskId id, Task task) override;
  OperationResult<Status> Complete(TaskId id) override;
  OperationResult<Status> Delete(TaskId id) override;

  OperationResult<Status, SolidTasks> GetAllSolidTasks() override;
  OperationResult<Status, SolidTasks> GetSpecificSolidTasks(
      std::vector<TaskId>) override;

  OperationResult<Status> Load() override;
  OperationResult<Status> Save() override;

 private:
  std::unique_ptr<TaskManager> task_manager_;
  std::unique_ptr<Persistence> persistence_;
};

std::optional<std::pair<TaskId, TaskId>> HasParentChildRelationship(
    const SolidTasks& tasks, const std::vector<TaskId>& ids);
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_MODEL_DEFAULTMODELCONTROLLER_H_
