#ifndef TASKMANAGER_SRC_REPL_PRINT_DEFAULTVALIDATOR_H_
#define TASKMANAGER_SRC_REPL_PRINT_DEFAULTVALIDATOR_H_

#include "repl/validator/IValidator.h"

namespace task_manager {
class DefaultValidator : public IValidator {
 public:
  std::pair<CommandEnum, std::string> MakeRequest(std::string str) override;
  std::string ConsumeOneTokenFrom(std::string &) override;
  std::optional<Task::Priority> ParseTaskPriority(
      const std::string &str) override;
  std::optional<Task::Progress> ParseTaskProgress(
      const std::string &str) override;
  std::optional<TaskDate_t> ParseTaskDate(const std::string &str) override;
  std::optional<ConfirmationResult> ParseConfirmation(
      const std::string &str) override;
  std::optional<std::string> ValidateTitle(const std::string &str) override;

  std::optional<int> ParseInt(const std::string &) override;

 private:
  CommandEnum MatchCommand(const std::string &str);
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_PRINT_DEFAULTVALIDATOR_H_
