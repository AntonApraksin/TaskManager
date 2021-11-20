#include "TaskManager.h"

TaskManager::TaskManager(std::unique_ptr<ITaskIdProducer> id_producer)
    : id_producer_(std::move(id_producer)) {}

TaskId TaskManager::Add(Task task) {
  auto next_id = id_producer_->GetNextId();
  storage_.Add(next_id, std::move(task));
  return next_id;
}

TaskManager& TaskManager::Edit(TaskId id, Task task) {
  storage_.Edit(id, std::move(task));
  return *this;
}

TaskManager& TaskManager::Complete(TaskId id) {
  storage_.Complete(id);
  return *this;
}

TaskManager& TaskManager::Delete(TaskId id) {
  storage_.Delete(id);
  return *this;
}

const TaskStorage::Storage& TaskManager::Show() const
{
  return storage_.Show();
}
