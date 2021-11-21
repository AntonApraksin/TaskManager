#include "TaskStorage.h"

void TaskStorage::Add(TaskId task_id, Task task) {
  children_.insert({task_id, {task}});
}

void TaskStorage::Add(TaskId parent, TaskId task_id, Task task) {
  FindImpl(parent)->Add(task_id, std::move(task));
}

void TaskStorage::Complete(TaskId task_id) { FindImpl(task_id)->Complete(); }

void TaskStorage::Delete(TaskId task_id) {
  if (auto it = children_.find(task_id); it != children_.end()) {
    children_.erase(it);
    return;
  }
  for (auto& i : children_) {
    i.second.Delete(task_id);
    return;
  }
  throw std::runtime_error("ID does not exist");
}

void TaskStorage::Edit(TaskId task_id, Task task) {
  FindImpl(task_id)->SetTask(std::move(task));
}

const TaskWrapper& TaskStorage::Find(TaskId task_id) const {
  return *FindImpl(task_id);
}

const TaskWrapper* TaskStorage::FindImpl(TaskId task_id) const {
  if (auto it = children_.find(task_id); it != children_.end()) {
    return &it->second;
  }
  for (auto& i : children_) {
    if (auto it = i.second.FindImpl(task_id); it != nullptr) {
      return it;
    }
  }
  throw std::runtime_error("ID does not exist");
}

TaskWrapper* TaskStorage::FindImpl(TaskId task_id) {
  if (auto it = children_.find(task_id); it != children_.end()) {
    return &it->second;
  }
  for (auto& i : children_) {
    if (auto it = i.second.FindImpl(task_id); it != nullptr) {
      return it;
    }
  }
  throw std::runtime_error("Unexisting ID");
}

const TaskStorage::Storage& TaskStorage::Show() const { return children_; }