#ifndef TASKMANAGER_SRC_MODEL_MODELCONTROLLER_H_
#define TASKMANAGER_SRC_MODEL_MODELCONTROLLER_H_

#include <istream>
#include <ostream>

#include "OperationResult.h"
#include "model/SolidTask.h"

namespace task_manager
{
class ModelController
{
public:
    enum class Status
    {
        kOk,
        kNotPresentId,
        kLoadFailure,
        kSaveFailure,
        kNotPresentLabel
    };

public:
    virtual OperationResult<Status, TaskId> AddTask(Task task) = 0;
    virtual OperationResult<Status, TaskId> AddSubtask(TaskId task_id, Task task) = 0;
    virtual OperationResult<Status> Edit(TaskId id, Task task) = 0;
    virtual OperationResult<Status> Complete(TaskId id) = 0;
    virtual OperationResult<Status> Delete(TaskId id) = 0;
    virtual OperationResult<Status, SolidTasks> GetAllSolidTasks() = 0;
    virtual OperationResult<Status, SolidTasks> GetSpecificSolidTasks(std::vector<TaskId>) = 0;
    virtual OperationResult<Status, SolidTasks> GetTasksByLabel(Label label) = 0;
    virtual OperationResult<Status> Load() = 0;
    virtual OperationResult<Status> Save() = 0;

    virtual OperationResult<Status> AddLabel(TaskId task_id, Label label) = 0;
    virtual OperationResult<Status> DeleteLabel(TaskId task_id, Label label) = 0;

    virtual ~ModelController() {}
};

}  // namespace task_manager

#endif  // TASKMANAGER_SRC_MODEL_MODELCONTROLLER_H_
