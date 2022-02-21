#ifndef TASKMANAGER_SRC_MODEL_DEFAULTMODELCONTROLLER_H_
#define TASKMANAGER_SRC_MODEL_DEFAULTMODELCONTROLLER_H_

#include <istream>
#include <mutex>
#include <ostream>

#include "OperationResult.h"
#include "model/ModelController.h"
#include "model/SolidTask.h"

namespace task_manager
{
class TaskManager;
class Persistence;

class DefaultModelController : public ModelController
{
public:
    explicit DefaultModelController(std::unique_ptr<TaskManager> task_manager,
                                    std::unique_ptr<Persistence> persistence);

    OperationResult<Status, TaskId> AddTask(Task task) override;
    OperationResult<Status, TaskId> AddSubtask(TaskId task_id, Task task) override;

    OperationResult<Status> Edit(TaskId id, Task task) override;
    OperationResult<Status> Complete(TaskId id) override;
    OperationResult<Status> Delete(TaskId id) override;

    OperationResult<Status, SolidTasks> GetAllSolidTasks() override;
    OperationResult<Status, SolidTasks> GetSpecificSolidTasks(std::vector<TaskId>) override;

    OperationResult<Status> Load() override;
    OperationResult<Status> Save() override;

    OperationResult<Status> AddLabel(TaskId task_id, Label label) override;
    OperationResult<Status> DeleteLabel(TaskId task_id, Label label) override;

private:
    std::unique_ptr<TaskManager> task_manager_;
    std::unique_ptr<Persistence> persistence_;
    std::mutex task_manager_mutex_;
    std::mutex persistence_mutex_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_MODEL_DEFAULTMODELCONTROLLER_H_
