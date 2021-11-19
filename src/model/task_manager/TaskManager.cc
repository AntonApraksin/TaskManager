#include "TaskManager.h"

TaskManager::TaskManager(std::unique_ptr<ITaskIdProducer> id_producer)
    : id_producer_(std::move(id_producer)) {}

TaskId TaskManager::Add(Task task) {
  auto next_id = id_producer_->GetNextId();
  tasks_.insert({next_id, std::move(task)});
  return next_id;
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
