#ifndef TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_INTERACTOR_STEPS_UTILS_VALIDATORUTILS_H_
#define TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_INTERACTOR_STEPS_UTILS_VALIDATORUTILS_H_

#include <optional>

#include "TaskId.pb.h"

namespace task_manager {
class IValidator;
std::optional<TaskId> ConsumeTaskIdFromString(IValidator& validator,
                                              std::string& str);
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_INTERACTOR_STEPS_UTILS_VALIDATORUTILS_H_
