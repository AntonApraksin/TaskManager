#include "Model/task_manager.h"

void TaskManager::Add(Task task) {
  if (tasks_.empty()) {
    tasks_.insert({0, std::move(task)});
  } else {
    tasks_.insert({tasks_.rbegin()->first + 1, std::move(task)});
  }
}

void TaskManager::Edit(int id, Task task) {
  if (auto to_edit = tasks_.find(id); to_edit == tasks_.end()) {
    throw std::runtime_error("Given id is invalid");
  } else {
    to_edit->second = std::move(task);
  }
}

void TaskManager::Complete(int id) {
  if (auto to_edit = tasks_.find(id); to_edit == tasks_.end()) {
    throw std::runtime_error("Given id is invalid");
  } else {
    tasks_.erase(to_edit);
  }
}

void TaskManager::Delete(int id) {
  if (auto to_edit = tasks_.find(id); to_edit == tasks_.end()) {
    throw std::runtime_error("Given id is invalid");
  } else {
    tasks_.erase(to_edit);
  }
}

std::map<int, Task> TaskManager::Show() { return tasks_; }
