#ifndef TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_INTERACTOR_STEPS_STEPPARAMETER_H_
#define TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_INTERACTOR_STEPS_STEPPARAMETER_H_

#include "interactor/state_machine/Context.h"

namespace task_manager
{
struct StepParameter
{
    task_manager::Context ctx;
    SolidTasks cache;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_INTERACTOR_STEPS_STEPPARAMETER_H_
