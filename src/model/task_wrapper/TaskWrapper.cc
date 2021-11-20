#include "TaskWrapper.h"

TaskWrapper::TaskWrapper(Task task) : task_(std::move(task)) {}

void TaskWrapper::Add(TaskId task_id, Task task) {
  children_.insert({task_id, {std::move(task)}});
}

void TaskWrapper::SetTask(Task task) { task_ = std::move(task); }

const Task& TaskWrapper::GetTask() const { return task_; }

const Task& TaskWrapper::operator*() const { return task_; }

void TaskWrapper::Complete() {
  task_ = *Task::Create(task_.GetTitle(), task_.GetPriority(),
                        task_.GetDueDate(), Task::State::kCompleted);
  for (auto& i : children_) {
    i.second.Complete();
  }
}

bool TaskWrapper::Delete(TaskId task_id) {
  if (auto it = children_.find(task_id); it != children_.end()) {
    children_.erase(it);
    return true;
  }
  for (auto& i : children_) {
    if (i.second.Delete(task_id)) {
      return true;
    }
  }
  return false;
}

TaskWrapper* TaskWrapper::Find(TaskId task_id) {
  if (auto it = children_.find(task_id); it != children_.end()) {
    return &it->second;
  }
  for (auto& i : children_) {
    if (auto it = i.second.Find(task_id); it != nullptr) {
      return it;
    }
  }
  return nullptr;
}

const TaskWrapper::Storage& TaskWrapper::Show() const { return children_; }
