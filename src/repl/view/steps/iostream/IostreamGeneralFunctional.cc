#include "IostreamGeneralFunctional.h"

#include <iomanip>
#include <iostream>

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

void ShowTask(const Task& task) {
  auto time = std::chrono::system_clock::to_time_t(task.GetDueDate());
  std::cout << " [" << to_string(task.GetState()) << "] "
            << "(" << to_string(task.GetPriority()) << ") "
            << "{" << std::put_time(std::localtime(&time), kDatePattern) << "} "
            << "'" << task.GetTitle() << "'\n";
}

void ShowTask(const Task& task, int nest) {
  std::string indent(nest, ' ');
  std::cout << indent;
  ShowTask(task);
}

void ShowTaskWithId(const Task& task, TaskId task_id) {
  std::cout << "└─ " << task_id.GetId() << ' ';
  ShowTask(task);
}

void ShowTaskWithId(const Task& task, TaskId task_id, int nest) {
  std::string indent(nest, ' ');
  std::cout << indent << "└─ " << task_id.GetId() << ' ';
  ShowTask(task);
}

void ShowNestedMap(const TaskWrapper& task_wrapper, int nest) {
  for (const auto& i : task_wrapper.ShowStorage()) {
    ShowTaskWithId(*(i.second), i.first, nest);
    ShowNestedMap(i.second, nest + 2);
  }
}

std::string PrintAndGet(const std::string& str) {
  std::cout << '[' << str << ']' << ": ";
  std::string result;
  std::getline(std::cin, result);
  return result;
}
