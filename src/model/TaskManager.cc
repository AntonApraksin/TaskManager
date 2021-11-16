#include "model/TaskManager.h"

TaskManager& TaskManager::Add(Task task) {
  tasks_.insert({id_producer_.GetNextId(), std::move(task)});
  return *this;
}

TaskManager& TaskManager::Edit(TaskId id, Task task) {
  if (auto to_edit = tasks_.find(id); to_edit == tasks_.end()) {
    throw std::runtime_error("Given id is invalid");
  } else {
    to_edit->second = std::move(task);
  }
  return *this;
}

TaskManager& TaskManager::Complete(TaskId id) {
  if (auto to_edit = tasks_.find(id); to_edit == tasks_.end()) {
    throw std::runtime_error("Given id is invalid");
  } else {
    tasks_.erase(to_edit);
  }
  return *this;
}

TaskManager& TaskManager::Delete(TaskId id) {
  if (auto to_edit = tasks_.find(id); to_edit == tasks_.end()) {
    throw std::runtime_error("Given id is invalid");
  } else {
    tasks_.erase(to_edit);
  }
  return *this;
}

const std::map<TaskId, Task>& TaskManager::Show() & { return tasks_; }
std::map<TaskId, Task> TaskManager::Show() && { return std::move(tasks_); }
