#ifndef TASKMANAGER_SRC_CLIENT_TASKSERVICEMODELCONTROLLER_H_
#define TASKMANAGER_SRC_CLIENT_TASKSERVICEMODELCONTROLLER_H_

#include <grpcpp/grpcpp.h>

#include <memory>

#include "TaskService.grpc.pb.h"
#include "model/ModelController.h"

namespace task_manager
{
class TaskServiceModelController : public ModelController
{
public:
    TaskServiceModelController(
        std::unique_ptr<task_manager::TaskService::StubInterface> stub);

    OperationResult<Status, TaskId> AddTask(Task task) override;

    OperationResult<Status, TaskId> AddSubtask(TaskId task_id, Task task) override;

    OperationResult<Status> Edit(TaskId id, Task task) override;

    OperationResult<Status> Complete(TaskId id) override;

    OperationResult<Status> Delete(TaskId id) override;

    OperationResult<Status, SolidTasks> GetAllSolidTasks() override;

    OperationResult<Status, SolidTasks> GetSpecificSolidTasks(
        std::vector<TaskId> vector) override;

    OperationResult<Status> Load() override;

    OperationResult<Status> Save() override;

    OperationResult<Status> AddLabel(TaskId task_id, Label label) override;

    OperationResult<Status> DeleteLabel(TaskId task_id, Label label) override;

private:
    std::unique_ptr<task_manager::TaskService::StubInterface> stub_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_CLIENT_TASKSERVICEMODELCONTROLLER_H_
