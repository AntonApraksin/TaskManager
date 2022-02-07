#ifndef TASKMANAGER_SRC_INTERACTOR_SMALL_STEPS_DEFAULT_CREATEFILLEDSMALLCONTEXT_H_
#define TASKMANAGER_SRC_INTERACTOR_SMALL_STEPS_DEFAULT_CREATEFILLEDSMALLCONTEXT_H_

#include "interactor/small_steps/ISmallStepFactory.h"

namespace task_manager {
struct TaskBuilder;

TaskContext CreateFilledTaskContext(ISmallStepFactory& small_step_factory,
                                    const TaskBuilder& task_builder);
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_INTERACTOR_SMALL_STEPS_DEFAULT_CREATEFILLEDSMALLCONTEXT_H_
