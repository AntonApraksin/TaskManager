#ifndef TASKMANAGER_SRC_MODEL_MODELCONTROLLER_H_
#define TASKMANAGER_SRC_MODEL_MODELCONTROLLER_H_

#include <istream>
#include <ostream>

#include "OperationResult.h"
#include "model/SolidTask.h"

namespace task_manager
{
/**
 * Public API for model.
 *
 * @author Anton Apraksin
 */
class ModelController
{
public:
    /**
     * Represents a set of possible error states.
     * that can occur during execution of any method.
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
     * Adds task to \b model.
     * @param task to add.
     * @return Contains \b TaskId given to new task or \b error.
     */
    virtual OperationResult<Status, TaskId> AddTask(Task task) = 0;

    /**
     * Adds subtask to task with given \a id.
     * @param task_id of parent.
     * @param task to add.
     * @return Contains \b TaskId given to new task or \b error.
     */
    virtual OperationResult<Status, TaskId> AddSubtask(TaskId task_id, Task task) = 0;

    /**
     * Replaces task with given \a id with new task.
     * @param task_id of task to edit.
     * @param task new task.
     * @return \b Status of operation.
     */
    virtual OperationResult<Status> Edit(TaskId task_id, Task task) = 0;

    /**
     * Completes task with given \a id and its children.
     * @param task_id of task to complete.
     * @return \b Status of operation.
     */
    virtual OperationResult<Status> Complete(TaskId task_id) = 0;

    /**
     * Deletes task with given \a id and its children.
     * @param task_id of task to delete.
     * @return \b Status of operation.
     */
    virtual OperationResult<Status> Delete(TaskId task_id) = 0;

    /**
     * Returns all tasks from \b model.
     * Tasks are sorted in the way so the caller can
     * restore the structure of the tree in <em>O(n)</em>.
     * Formally, order of tasks represents in-depth
     * tree traversal.
     * Order of tasks in not guaranteed, so task with id \a k
     * can be followed by task with id \a n so that <em>n < k</em>.
     * @return \b Sorted vector of tasks or error.
     */
    virtual OperationResult<Status, SolidTasks> GetAllSolidTasks() = 0;

    /**
     * Returns tasks with specified \a ids and their children.
     * Tasks are sorted in the same way as in \a GetAllSolidTasks.
     * @param task_ids vector of \a ids to show.
     * @return \b Sorted vector of tasks and their children or \b error.
     */
    virtual OperationResult<Status, SolidTasks> GetSpecificSolidTasks(std::vector<TaskId> task_ids) = 0;

    /**
     * Returns tasks with given \a label. Not sorted.
     * @param label to filter on.
     * @return Vector of tasks with given \a label or \b error.
     */
    virtual OperationResult<Status, SolidTasks> GetTasksByLabel(Label label) = 0;

    /**
     * Loads tasks from underlying storage.
     * Tasks that are currently in storage are \b erased.
     * @return \b Status of operation.
     */
    virtual OperationResult<Status> Load() = 0;

    /**
     * Saves tasks to underlying storage.
     * @return \b Status of operation.
     */
    virtual OperationResult<Status> Save() = 0;

    /**
     * Adds \a label to given task.
     * @param task_id of task to add label to.
     * @param label to add.
     * @return \b Status of operation.
     */
    virtual OperationResult<Status> AddLabel(TaskId task_id, Label label) = 0;

    /**
     * Deletes \a label from given task.
     * @param task_id of task to delete label from.
     * @param label to delete.
     * @return Status of operation.
     */
    virtual OperationResult<Status> DeleteLabel(TaskId task_id, Label label) = 0;

    virtual ~ModelController() {}
};

}  // namespace task_manager

#endif  // TASKMANAGER_SRC_MODEL_MODELCONTROLLER_H_
