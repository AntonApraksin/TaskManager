#include "TaskWrapper.h"

TaskWrapper::TaskWrapper(Task task) : task_(std::move(task)) {}

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