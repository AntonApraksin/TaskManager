#include "IostreamGeneralFunctional.h"

#include <iostream>

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
  std::cout << ' ' << " [" << to_string(task.GetState()) << "] "
            << "(" << to_string(task.GetPriority()) << ") " << task.GetTitle()
            << "\n";  // TODO: Display date
}

void ShowTask(const Task& task, int nest) {
  std::string indent(nest, ' ');
  std::cout << indent << "[" << to_string(task.GetState()) << "] "
            << "(" << to_string(task.GetPriority()) << ") " << task.GetTitle()
            << "\n";  // TODO: Display date
}

void ShowTaskWithId(const Task& task, TaskId task_id) {
  std::cout << "└─ " << task_id.GetId() << ' ';
  ShowTask(task, 0);
}

void ShowTaskWithId(const Task& task, TaskId task_id, int nest) {
  std::string indent(nest, ' ');
  std::cout << indent << "└─ " << task_id.GetId() << ' ';
  ShowTask(task, 0);
}

void ShowNestedMap(const TaskWrapper& task_wrapper, int nest) {
  for (const auto& i : task_wrapper.ShowStorage()) {
    ShowTaskWithId(*(i.second), i.first, nest);
    ShowNestedMap(i.second, nest + 2);
  }
}
