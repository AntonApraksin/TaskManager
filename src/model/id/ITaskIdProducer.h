#ifndef TASKMANAGER_SRC_MODEL_ID_ITASKIDPRODUCER_H_
#define TASKMANAGER_SRC_MODEL_ID_ITASKIDPRODUCER_H_

#include "TaskId.pb.h"

namespace task_manager
{
class ITaskIdProducer
{
public:
    virtual TaskId GetNextId() = 0;
    virtual TaskId GetCurrentId() = 0;

    virtual ~ITaskIdProducer() {};
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_MODEL_ID_ITASKIDPRODUCER_H_
