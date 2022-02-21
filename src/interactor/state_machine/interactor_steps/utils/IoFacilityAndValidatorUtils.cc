#include "IoFacilityAndValidatorUtils.h"

#include "interactor/io_facility/IIoFacility.h"
#include "interactor/io_facility/Strings.h"

namespace task_manager
{
std::optional<ConfirmationResult> ReadConfirmation(
    IIoFacility& io_facility, IValidator& validator,
    const std::string& confirmation_string)
{
    io_facility.Print(Strings::ProceedTo(confirmation_string));
    std::string input = io_facility.GetLine();
    return validator.ParseConfirmation(input);
}
}  // namespace task_manager