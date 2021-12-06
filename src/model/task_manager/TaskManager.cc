#include "TaskManager.h"

TaskManager::TaskManager(std::unique_ptr<ITaskIdProducer> id_producer)
    : id_producer_(std::move(id_producer)) {}

TaskId TaskManager::Add(Task task) {
  auto next_id = id_producer_->GetNextId();
  storage_.Add(next_id, std::move(task));
  return next_id;
}

TaskId TaskManager::Add(TaskId task_id, Task task) {
  auto next_id = id_producer_->GetNextId();
  auto& add_to = storage_.Find(task_id);
  add_to.Add(next_id, std::move(task));
  return next_id;
}

TaskManager& TaskManager::Edit(TaskId id, Task task) {
  auto& to_edit = storage_.Find(id);
  to_edit.SetTask(task);
  return *this;
}

TaskManager& TaskManager::Complete(TaskId id) {
  auto& to_edit = storage_.Find(id);
  to_edit.Complete();
  return *this;
}

TaskManager& TaskManager::Delete(TaskId id) {
  auto& parent_of = storage_.FindParentOf(id);
  parent_of.Delete(id);
  return *this;
}

TaskStorage TaskManager::Show() const { return storage_; }