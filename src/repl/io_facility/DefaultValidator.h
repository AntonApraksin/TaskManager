#ifndef TASKMANAGER_SRC_REPL_PRINT_DEFAULTVALIDATOR_H_
#define TASKMANAGER_SRC_REPL_PRINT_DEFAULTVALIDATOR_H_

#include "repl/io_facility/IValidator.h"

class DefaultValidator : public IValidator {
 public:
  std::pair<StateEnum, std::vector<TaskId>> MakeRequest(
      const std::string &str) override;
  std::optional<Task::Priority> ParseTaskPriority(
      const std::string &str) override;

  std::optional<Task::State> ParseTaskState(const std::string &str) override;
  std::optional<Date_t> ParseTaskDate(const std::string &str) override;
  std::optional<ConfirmationResult> ParseConfirmation(
      const std::string &str) override;

  std::optional<std::string> ValidateTitle(const std::string &str) override;

 private:
  StateEnum MatchState(const std::string &str);
};

#endif  // TASKMANAGER_SRC_REPL_PRINT_DEFAULTVALIDATOR_H_
