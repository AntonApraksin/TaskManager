#include "TaskManager.h"

// TODO: return errors on failure

TaskManager::TaskManager(std::unique_ptr<ITaskIdProducer> id_producer)
    : id_producer_(std::move(id_producer)) {}

std::pair<std::optional<TaskId>, TaskManager::ActionResult> TaskManager::Add(
    Task task) {
  auto next_id = id_producer_->GetNextId();
  storage_.Add(next_id, std::move(task));
  return {next_id, ActionResult::ok()};
}

std::pair<std::optional<TaskId>, TaskManager::ActionResult> TaskManager::Add(
    TaskId task_id, Task task) {
  auto next_id = id_producer_->GetNextId();
  auto add_to = storage_.Find(task_id);
  if (add_to != storage_.end()) {
    add_to->second.Add(next_id, std::move(task));
    return {next_id, ActionResult::ok()};
  }
  return {{}, ActionResult::error(ActionResult::Status::kNotPresentId)};
}

TaskManager::ActionResult TaskManager::Edit(TaskId id, Task task) {
  auto to_edit = storage_.Find(id);
  if (to_edit != storage_.end()) {
    to_edit->second.SetTask(task);
    return ActionResult::ok();
  }
  return ActionResult::error(ActionResult::Status::kNotPresentId);
}

TaskManager::ActionResult TaskManager::Complete(TaskId id) {
  auto to_complete = storage_.Find(id);
  if (to_complete != storage_.end()) {
    to_complete->second.Complete();
    return ActionResult::ok();
  }
  return ActionResult::error(ActionResult::Status::kNotPresentId);
}

TaskManager::ActionResult TaskManager::Delete(TaskId id) {
  auto parent_of = storage_.FindStorageContaining(id);
  if (parent_of) {
    parent_of->get().Delete(parent_of->get().Find(id));
    return ActionResult::ok();
  }
  return ActionResult::error(ActionResult::Status::kNotPresentId);
}

TaskStorage TaskManager::Show() const { return storage_; }