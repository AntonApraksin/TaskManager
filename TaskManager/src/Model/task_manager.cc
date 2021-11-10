#include "Model/task_manager.h"

void TaskManager::Add(Task task) {
  tasks_.insert({id_producer_.GetNextId(), std::move(task)});
}

void TaskManager::Edit(TaskId id, Task task) {
  if (auto to_edit = tasks_.find(id); to_edit == tasks_.end()) {
    throw std::runtime_error("Given id is invalid");
  } else {
    to_edit->second = std::move(task);
  }
}

void TaskManager::Complete(TaskId id) {
  if (auto to_edit = tasks_.find(id); to_edit == tasks_.end()) {
    throw std::runtime_error("Given id is invalid");
  } else {
    tasks_.erase(to_edit);
  }
}

void TaskManager::Delete(TaskId id) {
  if (auto to_edit = tasks_.find(id); to_edit == tasks_.end()) {
    throw std::runtime_error("Given id is invalid");
  } else {
    tasks_.erase(to_edit);
  }
}

const std::map<TaskId, Task>& TaskManager::Show() { return tasks_; }
