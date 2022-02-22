#ifndef TASKMANAGER_SRC_MODEL_MODELCONTROLLER_H_
#define TASKMANAGER_SRC_MODEL_MODELCONTROLLER_H_

#include <istream>
#include <ostream>

#include "OperationResult.h"
#include "model/SolidTask.h"

namespace task_manager
{
/**
 * Public API for model
 *
 * @author Anton Apraksin
 */
class ModelController
{
public:
    /**
     * Represents a set of possible error states
     * that can occur
     */
    enum class Status
    {
        kOk,
        kNotPresentId,
        kLoadFailure,
        kSaveFailure,
        kNotPresentLabel
    };

public:
    /**
     * Adds task to model
     * @param task to add
     * @return Contains TaskId given to new task or error
     */
    virtual OperationResult<Status, TaskId> AddTask(Task task) = 0;

    /**
     * Adds subtask to task with given id
     * @param task_id of parent
     * @param task to add
     * @return Contains TaskId given to new task or error.
     */
    virtual OperationResult<Status, TaskId> AddSubtask(TaskId task_id, Task task) = 0;

    /**
     * Replaces task with given id with new task
     * @param task_id of task to edit
     * @param task new task
     * @return Status of operation
     */
    virtual OperationResult<Status> Edit(TaskId task_id, Task task) = 0;

    /**
     * Completes task with given id and its children
     * @param task_id of task to complete
     * @return Status of operation
     */
    virtual OperationResult<Status> Complete(TaskId task_id) = 0;

    /**
     * Deletes task with given id and its children
     * @param task_id
     * @return Status of operation
     */
    virtual OperationResult<Status> Delete(TaskId task_id) = 0;

    /**
     * Returns all tasks from model
     * Tasks are sorted in the way so the caller can
     * restore the structure of the tree in O(n)
     * Formally, order of tasks represents in-depth
     * tree traversal.
     * Order of tasks in not guaranteed, so task with id k
     * can be followed by task with id n so that n < k
     * Example:
     * task_id   | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
     * parent_id | - | - | 1 | 2 | 2 | 4 | 1 | - |
     * @return Sorted vector of tasks
     */
    virtual OperationResult<Status, SolidTasks> GetAllSolidTasks() = 0;

    /**
     * Returns tasks with specified ids and their children
     * Tasks are sorted in the same way as in GetAllSolidTasks
     * @param task_ids vector of ids to show
     * @return Sorted vector of tasks and their children
     */
    virtual OperationResult<Status, SolidTasks> GetSpecificSolidTasks(std::vector<TaskId> task_ids) = 0;

    /**
     * Returns tasks with given label. Not sorted.
     * @param label to filter on
     * @return Vector of tasks with given label
     */
    virtual OperationResult<Status, SolidTasks> GetTasksByLabel(Label label) = 0;

    /**
     * Loads tasks from underlying storage
     * Tasks that are currently in storage are erased
     * @return Status of operation
     */
    virtual OperationResult<Status> Load() = 0;

    /**
     * Saves tasks to underlying storage
     * @return Status of operation
     */
    virtual OperationResult<Status> Save() = 0;

    /**
     * Adds label to given task
     * @param task_id of task to add label to
     * @param label to add
     * @return Status of operation
     */
    virtual OperationResult<Status> AddLabel(TaskId task_id, Label label) = 0;

    /**
     * Deletes label from given task
     * @param task_id of task to delete label from
     * @param label to delete
     * @return Status of operation
     */
    virtual OperationResult<Status> DeleteLabel(TaskId task_id, Label label) = 0;

    virtual ~ModelController() {}
};

}  // namespace task_manager

#endif  // TASKMANAGER_SRC_MODEL_MODELCONTROLLER_H_
