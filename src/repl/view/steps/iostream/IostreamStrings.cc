#include "IostreamStrings.h"

#include <google/protobuf/util/time_util.h>

#include <iomanip>
#include <sstream>

#include "repl/validator/DateFormat.h"

const char* IostreamStrings::to_string(Task::Priority priority) {
  if (priority == Task::kLow) return "low";
  if (priority == Task::kMedium) return "medium";
  if (priority == Task::kHigh) return "high";

  std::terminate();  // TODO: Handle buggy behavior
}

const char* IostreamStrings::to_string(Task::Progress state) {
  if (state == Task::kCompleted) return "+";
  if (state == Task::kUncompleted) return "-";

  std::terminate();  // TODO: Handle buggy behavior
}

std::string IostreamStrings::ShowTask(const Task& task) {
  std::stringstream ss;
  auto time =
      google::protobuf::util::TimeUtil::TimestampToTimeT(task.due_date());
  auto localized_time = std::localtime(&time);
  ss << " [" << to_string(task.progress()) << "] "
     << "(" << to_string(task.priority()) << ") "
     << "{" << std::put_time(localized_time, kDatePattern) << "} "
     << "'" << task.title() << "'\n";
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
  ss << "└─ " << task_id.id() << ' ' << ShowTask(task);
  return ss.str();
}

std::string IostreamStrings::ShowTaskWithId(const Task& task, TaskId task_id,
                                            int nest) {
  std::stringstream ss;
  std::string indent(nest, ' ');
  ss << indent << "└─ " << task_id.id() << ' ' << ShowTask(task);
  return ss.str();
}

std::string IostreamStrings::ShowNestedMap(const TaskWrapper& task_wrapper,
                                           int nest) {
  std::stringstream ss;
  for (const auto& i : task_wrapper.ShowStorage()) {
    ss << ShowTaskWithId(*(i.second), i.first, nest)
       << ShowNestedMap(i.second, nest + 2);
  }
  return ss.str();
}

std::string IostreamStrings::NotPresentId(int id) {
  std::stringstream ss;
  ss << "Id " << id << " not present in store.\n";
  return ss.str();
}

std::string IostreamStrings::ShowId(int id) {
  std::stringstream ss;
  ss << "Id " << id << ".\n";
  return ss.str();
}

std::string IostreamStrings::YouAreGoingTo(const std::string& str) {
  std::stringstream ss;
  ss << "You are going to " << str << " such tasks:\n";
  return ss.str();
}

std::string IostreamStrings::AndItsChildren(size_t n) {
  std::stringstream ss;
  ss << "  and its " << n << " children.\n";
  return ss.str();
}

std::string IostreamStrings::LeaveEmptyFor(const std::string& str) {
  std::stringstream ss;
  ss << "Leave empty for '" << str << "'\n";
  return ss.str();
}

std::string IostreamStrings::LeaveEmptyFor(const std::string& str1,
                                           const std::string& str2) {
  std::stringstream ss;
  ss << "Leave empty for '" << str1 << "'. " << str2 << "\n";
  return ss.str();
}

std::string IostreamStrings::GetPrompt(const std::string& str) {
  std::stringstream ss;
  ss << '[' << str << "]: ";
  return ss.str();
}

std::string IostreamStrings::GetPrompt(const std::string& str1,
                                       const std::string& str2) {
  std::stringstream ss;
  ss << '[' << str1 << "(" << str2 << ")]: ";
  return ss.str();
}

std::string IostreamStrings::ProceedTo(const std::string& str) {
  std::stringstream ss;
  ss << "Proceed to " << str << "? [Y/n]: ";
  return ss.str();
}

std::string PrintAndGet(IIoFacility& io, const std::string& str) {
  io.Print(str);
  std::string input = io.GetLine();
  return input;
}