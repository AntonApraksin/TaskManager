#ifndef TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_STEPEVENT_H_
#define TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_STEPEVENT_H_

namespace task_manager
{
enum class StepEvent
{
    kNothing,
    kNotPresentId,
    kLoadFailure,
    kSaveFailure,
    kNotPresentLabel,

    kShowByLabel,
    kShowAll,
    kShowById,
    kShowWithoutNest,
    kShowId
};
}

#endif  // TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_STEPEVENT_H_
