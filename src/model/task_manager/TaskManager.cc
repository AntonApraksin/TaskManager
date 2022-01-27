#include "TaskManager.h"

namespace task_manager {
void GetCompleteSubTree(TaskId id, const TaskManager::Parents& parents,
                        std::vector<TaskId>& children) {
  const auto it = parents.find(id);
  const auto& to_traverse = it->second;
  children.push_back(id);
  for (size_t i{0}; i != to_traverse.size(); ++i) {
    GetCompleteSubTree(to_traverse[i], parents, children);
  }
}

using TMStatus = TaskManager::Status;

TaskManager::TaskManager(std::unique_ptr<ITaskIdProducer> id_producer)
    : id_producer_(std::move(id_producer)) {}

TaskManager::TaskManager(std::unique_ptr<ITaskIdProducer> id_producer,
                         TaskManager::Storage storage)
    : storage_(std::move(storage)), id_producer_(std::move(id_producer)) {}

OperationResult<TMStatus, TaskId> TaskManager::Add(Task task) {
  auto next_id = id_producer_->GetNextId();
  storage_.tasks.insert({next_id, std::move(task)});
  storage_.roots.push_back(next_id);
  storage_.parents[next_id];
  return OperationResult<Status, TaskId>::Ok(next_id);
}

OperationResult<TMStatus, TaskId> TaskManager::Add(TaskId id, Task task) {
  if (auto it = storage_.parents.find(id); it != storage_.parents.end()) {
    auto next_id = id_producer_->GetNextId();
    storage_.tasks.insert({next_id, std::move(task)});
    it->second.push_back(next_id);
    storage_.parents[next_id];
    return OperationResult<Status, TaskId>::Ok(next_id);
  }
  return OperationResult<Status, TaskId>::Error(Status::kNotPresentId);
}

OperationResult<TMStatus> TaskManager::Edit(TaskId id, Task task) {
  if (auto it = storage_.tasks.find(id); it != storage_.tasks.end()) {
    it->second = std::move(task);
    return OperationResult<Status>::Ok();
  }
  return OperationResult<Status>::Error(Status::kNotPresentId);
}

OperationResult<TMStatus> TaskManager::Complete(TaskId id) {
  if (auto it = storage_.tasks.find(id); it != storage_.tasks.end()) {
    std::vector<TaskId> to_complete;
    GetCompleteSubTree(id, storage_.parents, to_complete);
    for (const auto& i : to_complete) {
      storage_.tasks.find(i)->second.set_progress(Task::kCompleted);
    }
    return OperationResult<Status>::Ok();
  }
  return OperationResult<Status>::Error(Status::kNotPresentId);
}

OperationResult<TMStatus> TaskManager::Delete(TaskId id) {
  if (auto it = storage_.tasks.find(id); it != storage_.tasks.end()) {
    std::vector<TaskId> to_delete;
    GetCompleteSubTree(id, storage_.parents, to_delete);
    for (const auto& i : to_delete) {
      storage_.tasks.erase(storage_.tasks.find(i));
      storage_.parents.erase(storage_.parents.find(i));
      if (auto root =
              std::find(storage_.roots.begin(), storage_.roots.end(), i);
          root != storage_.roots.end()) {
        storage_.roots.erase(root);
      } else {
        for (auto& potential_parent : storage_.parents) {
          if (auto parent = std::find(potential_parent.second.begin(),
                                      potential_parent.second.end(), id);
              parent != potential_parent.second.end()) {
            potential_parent.second.erase(parent);
            break;
          }
        }
      }
    }
    return OperationResult<Status>::Ok();
  }
  return OperationResult<Status>::Error(Status::kNotPresentId);
}

OperationResult<TMStatus> TaskManager::AddLabel(TaskId id, Label label) {
  if (auto it = storage_.tasks.find(id); it != storage_.tasks.end()) {
    auto& task = it->second;
    if (std::find_if(task.labels().begin(), task.labels().end(),
                     [&label](auto& stored_lable) {
                       return label.name() == stored_lable.name();
                     }) == task.labels().end()) {
      auto new_label = task.add_labels();
      *new_label = std::move(label);
    }
    return OperationResult<Status>::Ok();
  }
  return OperationResult<Status>::Error(Status::kNotPresentId);
}

OperationResult<TMStatus> TaskManager::DeleteLabel(TaskId id, Label label) {
  if (auto it = storage_.tasks.find(id); it != storage_.tasks.end()) {
    auto& task = it->second;
    if (auto to_erase = std::find_if(task.labels().begin(), task.labels().end(),
                                     [&label](auto& stored_lable) {
                                       return label.name() ==
                                              stored_lable.name();
                                     });
        to_erase != task.labels().end()) {
      task.mutable_labels()->erase(to_erase);
      return OperationResult<Status>::Ok();
    }
    return OperationResult<Status>::Error(Status::kNotPresentLabel);
  }
  return OperationResult<Status>::Error(Status::kNotPresentId);
}

OperationResult<TMStatus, TaskManager::Storage> TaskManager::Show() const {
  return OperationResult<Status, Storage>::Ok(storage_);
}
}  // namespace task_manager
