#include "ValidatorUtils.h"

#include "interactor/validator/IValidator.h"
#include "utils/TaskIdUtils.h"

namespace task_manager {

std::optional<TaskId> ConsumeTaskIdFromString(IValidator& validator,
                                              std::string& str) {
  auto token = validator.ConsumeOneTokenFrom(str);
  auto id = validator.ParseInt(token);
  if (id) {
    return CreateTaskId(*id);
  }
  return {};
}
}  // namespace task_manager