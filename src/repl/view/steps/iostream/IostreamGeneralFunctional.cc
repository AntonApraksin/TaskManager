#include "IostreamGeneralFunctional.h"

#include <iomanip>
#include <sstream>

#include "repl/validator/DateFormat.h"

const char* to_string(Task::Priority priority) {
  switch (priority) {
    case Task::Priority::kLow:
      return "low";
    case Task::Priority::kMedium:
      return "medium";
    case Task::Priority::kHigh:
      return "high";
  }
}

const char* to_string(Task::State state) {
  switch (state) {
    case Task::State::kCompleted:
      return "+";
    case Task::State::kUncompleted:
      return "-";
  }
}

void ShowTask(IIoFacility& io, const Task& task) {
  std::stringstream ss;
  auto time = std::chrono::system_clock::to_time_t(task.GetDueDate());
  ss << " [" << to_string(task.GetState()) << "] "
     << "(" << to_string(task.GetPriority()) << ") "
     << "{" << std::put_time(std::localtime(&time), kDatePattern) << "} "
     << "'" << task.GetTitle() << "'\n";
  io.Print(ss.str());
}

void ShowTask(IIoFacility& io, const Task& task, int nest) {
  std::string indent(nest, ' ');
  io.Print(indent);
  ShowTask(io, task);
}

void ShowTaskWithId(IIoFacility& io, const Task& task, TaskId task_id) {
  std::stringstream ss;
  ss << "└─ " << task_id.GetId() << ' ';
  io.Print(ss.str());
  ShowTask(io, task);
}

void ShowTaskWithId(IIoFacility& io, const Task& task, TaskId task_id,
                    int nest) {
  std::stringstream ss;
  std::string indent(nest, ' ');
  ss << indent << "└─ " << task_id.GetId() << ' ';
  io.Print(ss.str());
  ShowTask(io, task);
}

void ShowNestedMap(IIoFacility& io, const TaskWrapper& task_wrapper, int nest) {
  for (const auto& i : task_wrapper.ShowStorage()) {
    ShowTaskWithId(io, *(i.second), i.first, nest);
    ShowNestedMap(io, i.second, nest + 2);
  }
}

std::string PrintAndGet(IIoFacility& io, const std::string& str) {
  std::stringstream ss;
  ss << '[' << str << ']' << ": ";
  io.Print(ss.str());
  std::string result = io.GetLine();
  return result;
}
