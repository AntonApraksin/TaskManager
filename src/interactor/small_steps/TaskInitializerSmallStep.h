#ifndef TASKMANAGER_SRC_REPL_VIEW_SMALL_STEP_TASK_INITIALIZER_SMALL_STEP_TASKINITIALIZERSMALLSTEP_H_
#define TASKMANAGER_SRC_REPL_VIEW_SMALL_STEP_TASK_INITIALIZER_SMALL_STEP_TASKINITIALIZERSMALLSTEP_H_

#include "ISmallStep.h"
#include "utils/TaskBuilder.h"

namespace task_manager
{
class DefaultTaskInitializerSmallStep : public ISmallStep
{
public:
    explicit DefaultTaskInitializerSmallStep(const TaskBuilder&);
    void Execute(TaskContext& ctx) override;

private:
    TaskBuilder task_builder_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_VIEW_SMALL_STEP_TASK_INITIALIZER_SMALL_STEP_TASKINITIALIZERSMALLSTEP_H_
