#include "TaskStorage.h"

TaskStorage::TaskStorage()
: children_() {}

void TaskStorage::Add(TaskId task_id, Task task) {
  children_.insert({task_id, {task}});
  ++size_;
}

void TaskStorage::Add(TaskId parent, TaskId task_id, Task task) {
  if (auto found = Find(parent); found != nullptr) {
    found->Add(task_id, std::move(task));
    ++size_;
  }
}

void TaskStorage::Complete(TaskId task_id) {
  if (auto found = Find(task_id); found != nullptr) {
    found->Complete();
  }
}
void TaskStorage::Delete(TaskId task_id) {
  if (auto it = children_.find(task_id); it != children_.end()) {
    children_.erase(it);
    --size_;
    return;
  }
  for (auto& i : children_) {
    if (i.second.Delete(task_id)) {
      --size_;
      return;
    }
  }
  throw std::runtime_error("Unexisting ID");
}

void TaskStorage::Edit(TaskId task_id, Task task) {
  if (auto found = Find(task_id); found != nullptr) {
    found->SetTask(std::move(task));
  }
}

TaskWrapper* TaskStorage::Find(TaskId task_id) {
  if (auto it = children_.find(task_id); it != children_.end()) {
    return &it->second;
  }
  for (auto& i : children_) {
    if (auto it = i.second.Find(task_id); it != nullptr) {
      return it;
    }
  }
  throw std::runtime_error("Unexisting ID");
}

const TaskStorage::Storage& TaskStorage::Show() const { return children_; }