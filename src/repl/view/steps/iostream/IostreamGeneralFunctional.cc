#include "IostreamGeneralFunctional.h"

#include <iomanip>
#include <sstream>

#include "repl/validator/DateFormat.h"

const char* IostreamStrings::to_string(Task::Priority priority) {
  switch (priority) {
    case Task::Priority::kLow:
      return "low";
    case Task::Priority::kMedium:
      return "medium";
    case Task::Priority::kHigh:
      return "high";
  }
}

const char* IostreamStrings::to_string(Task::State state) {
  switch (state) {
    case Task::State::kCompleted:
      return "+";
    case Task::State::kUncompleted:
      return "-";
  }
}

std::string IostreamStrings::ShowTask(const Task& task) {
  std::stringstream ss;
  auto time = std::chrono::system_clock::to_time_t(task.GetDueDate());
  ss << " [" << to_string(task.GetState()) << "] "
     << "(" << to_string(task.GetPriority()) << ") "
     << "{" << std::put_time(std::localtime(&time), kDatePattern) << "} "
     << "'" << task.GetTitle() << "'\n";
  return ss.str();
}

std::string IostreamStrings::ShowTask(const Task& task, int nest) {
  std::stringstream ss;
  std::string indent(nest, ' ');
  ss << indent << ShowTask(task);
  return ss.str();
}

std::string IostreamStrings::ShowTaskWithId(const Task& task, TaskId task_id) {
  std::stringstream ss;
  ss << "└─ " << task_id.GetId() << ' ' << ShowTask(task);
  return ss.str();
}

std::string IostreamStrings::ShowTaskWithId(const Task& task, TaskId task_id,
                    int nest) {
  std::stringstream ss;
  std::string indent(nest, ' ');
  ss << indent << "└─ " << task_id.GetId() << ' ' << ShowTask(task);
  return ss.str();
}

std::string IostreamStrings::ShowNestedMap(const TaskWrapper& task_wrapper, int nest) {
  std::stringstream ss;
  for (const auto& i : task_wrapper.ShowStorage()) {
    ss << ShowTaskWithId(*(i.second), i.first, nest)
    << ShowNestedMap(i.second, nest + 2);
  }
  return ss.str();
}

std::string IostreamStrings::NotPresentId(int id)
{
  std::stringstream ss;
  ss << "Id " << id << " not present in store.\n";
  return ss.str();
}

std::string IostreamStrings::ShowId(int id)
{
  std::stringstream ss;
  ss << "Id " << id << ".\n";
  return ss.str();
}

std::string IostreamStrings::YouAreGoingTo(const std::string& str)
{
  std::stringstream ss;
  ss << "You are going to " << str << " such tasks:\n";
  return ss.str();
}

std::string IostreamStrings::AndItsChildren(int n)
{
  std::stringstream ss;
  ss << "  and its " << n << " children.\n";
  return ss.str();
}

std::string IostreamStrings::LeaveEmptyFor(const std::string& str)
{
  std::stringstream ss;
  ss << "Leave empty for '" << str << "'\n";
  return ss.str();
}

std::string IostreamStrings::LeaveEmptyFor(const std::string& str1, const std::string& str2)
{
  std::stringstream ss;
  ss << "Leave empty for '" << str1 << "'. " << str2 << "\n";
  return ss.str();
}

std::string IostreamStrings::GetPrompt(const std::string &str)
{
  std::stringstream ss;
  ss << '[' << str << "]: ";
  return ss.str();
}

std::string IostreamStrings::GetPrompt(const std::string &str1, const std::string &str2)
{
  std::stringstream ss;
  ss << '[' << str1 << "(" << str2 << ")]: ";
  return ss.str();
}

std::string IostreamStrings::ProceedTo(const std::string& str)
{
  std::stringstream ss;
  ss << "Proceed to " << str << "? [Y/n]: ";
  return ss.str();
}
