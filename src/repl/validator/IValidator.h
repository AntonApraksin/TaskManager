#ifndef TASKMANAGER_SRC_REPL_PRINT_IVALIDATOR_H_
#define TASKMANAGER_SRC_REPL_PRINT_IVALIDATOR_H_

#include <optional>
#include <string>

#include "repl/state_machine/repl_steps/CommandEnum.h"
#include "utils/TaskUtils.h"

namespace task_manager {
enum class ConfirmationResult {
  kYes,
  kNo,
};

class IValidator {
 public:
  virtual std::pair<CommandEnum, std::string> MakeRequest(std::string str) = 0;

  virtual std::string ConsumeOneTokenFrom(std::string&) = 0;

  virtual std::optional<Task::Priority> ParseTaskPriority(
      const std::string& str) = 0;

  virtual std::optional<Task::Progress> ParseTaskProgress(
      const std::string& str) = 0;

  virtual std::optional<TaskDate_t> ParseTaskDate(const std::string& str) = 0;
  virtual std::optional<ConfirmationResult> ParseConfirmation(
      const std::string& str) = 0;

  virtual std::optional<std::string> ValidateTitle(const std::string& str) = 0;

  virtual std::optional<int> ParseInt(const std::string&) = 0;

  virtual ~IValidator() {}
};
}  // namespace task_manager
#endif  // TASKMANAGER_SRC_REPL_PRINT_IVALIDATOR_H_
