#ifndef TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_INTERACTOR_STEPS_UTILS_IOFACILITYANDVALIDATORUTILS_H_
#define TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_INTERACTOR_STEPS_UTILS_IOFACILITYANDVALIDATORUTILS_H_

#include "interactor/validator/IValidator.h"

namespace task_manager
{
class IIoFacility;

std::optional<ConfirmationResult> ReadConfirmation(
    IIoFacility& io_facility, IValidator& validator,
    const std::string& confirmation_string);
}  // namespace task_manager
#endif  // TASKMANAGER_SRC_INTERACTOR_STATE_MACHINE_INTERACTOR_STEPS_UTILS_IOFACILITYANDVALIDATORUTILS_H_
