#include "DefaultValidator.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>

void lower_string(std::string &str) {
  std::transform(str.cbegin(), str.cend(), str.begin(),
                 [](auto c) { return std::tolower(c); });
}

StateEnum DefaultValidator::MatchState(const std::string &str) {
  std::string input(str);
  lower_string(input);
  if (input == "a" || input == "add") {
    return StateEnum::kAdd;
  }
  if (input == "ex" || input == "exit") {
    return StateEnum::kExit;
  }
  if (input == "h" || input == "help") {
    return StateEnum::kHelp;
  }
  if (input == "d" || input == "delete") {
    return StateEnum::kDelete;
  }
  if (input == "c" || input == "complete") {
    return StateEnum::kComplete;
  }
  if (input == "ed" || input == "edit") {
    return StateEnum::kEdit;
  }
  return StateEnum::kUnknown;
}

std::optional<Task::Priority> DefaultValidator::ParseTaskPriority(
    const std::string &str) {
  std::string input(str);
  lower_string(input);

  if (input == "high") return Task::Priority::kHigh;
  if (input == "medium") return Task::Priority::kMedium;
  if (input == "low") return Task::Priority::kLow;
  return std::nullopt;
}

std::optional<Date_t> DefaultValidator::ParseTaskDate(const std::string &str) {
  // TODO make if return nullopt. get_time is stupid
  std::tm tm = {};
  std::string pattern(
      "%H:%M %d/%m/%Y");  // TODO: make single format across all program
  std::istringstream ss(str);
  ss >> std::get_time(&tm, pattern.c_str());
  if (ss.fail()) {
    return std::nullopt;
  }
  auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
  return tp;
}
std::optional<TaskId> DefaultValidator::ParseTaskId(const std::string &str) {
  try {
    return TaskId::Create(std::stoi(str));
  } catch (const std::invalid_argument &) {
    return std::nullopt;
  }
}

std::optional<ConfirmationResult> DefaultValidator::ParseConfirmation(
    const std::string &str) {
  std::string input(str);
  lower_string(input);
  if (str == "y" || str == "yes") {
    return ConfirmationResult::kYes;
  }
  if (str == "n" || str == "no") {
    return ConfirmationResult::kNo;
  }
  return std::nullopt;
}

std::optional<std::string> DefaultValidator::ValidateTitle(
    const std::string &str) {
  if (str.empty()) {
    return std::nullopt;
  } else {
    return str;
  }
}