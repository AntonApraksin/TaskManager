#include "ModelController.h"

#include "model/task_manager/TaskManager.h"
#include "persistence/Persistence.h"
#include "utils/TaskIdUtils.h"

namespace task_manager {
using MCStatus = ModelController::Status;

MCStatus TMStatusToMCStatus(TaskManager::Status tmstatus) {
  switch (tmstatus) {
    case TaskManager::Status::kNotPresentId:
      return MCStatus::kNotPresentId;

    case TaskManager::Status::kOk:
      return MCStatus::kOk;
  }
}

ModelController::ModelController(std::unique_ptr<TaskManager> task_manager)
    : task_manager_(std::move(task_manager)) {}

OperationResult<MCStatus, TaskId> ModelController::Add(Task task) {
  auto result = task_manager_->Add(std::move(task));
  if (result) {
    return OperationResult<Status, TaskId>::Ok(result.AccessResult());
  }
  return OperationResult<Status, TaskId>::Error(
      TMStatusToMCStatus(result.GetStatus()));
}

OperationResult<MCStatus, TaskId> ModelController::Add(TaskId id, Task task) {
  auto result = task_manager_->Add(std::move(id), std::move(task));
  if (result) {
    return OperationResult<Status, TaskId>::Ok(result.AccessResult());
  }
  return OperationResult<Status, TaskId>::Error(
      TMStatusToMCStatus(result.GetStatus()));
}

OperationResult<MCStatus> ModelController::Edit(TaskId id, Task task) {
  auto result = task_manager_->Edit(std::move(id), std::move(task));
  if (result) {
    return OperationResult<Status>::Ok();
  }
  return OperationResult<Status>::Error(TMStatusToMCStatus(result.GetStatus()));
}

OperationResult<MCStatus> ModelController::Complete(TaskId id) {
  auto result = task_manager_->Complete(std::move(id));
  if (result) {
    return OperationResult<Status>::Ok();
  }
  return OperationResult<Status>::Error(TMStatusToMCStatus(result.GetStatus()));
}

OperationResult<MCStatus> ModelController::Delete(TaskId id) {
  auto result = task_manager_->Delete(std::move(id));
  if (result) {
    return OperationResult<Status>::Ok();
  }
  return OperationResult<Status>::Error(TMStatusToMCStatus(result.GetStatus()));
}

// Precondition: All ids must be present in store
SolidTasks GetSolidTasksSorted(TaskManager::Storage storage) {
  if (storage.roots.empty()) {
    return {};
  }
  auto& roots = storage.roots;
  auto& parents = storage.parents;
  auto& tasks = storage.tasks;
  SolidTasks result;
  std::vector<TaskId> to_traverse{storage.roots.back()};
  storage.roots.pop_back();
  bool is_growing = true;
  for (; !to_traverse.empty();) {
    if (is_growing and tasks.find(to_traverse.back()) != tasks.end()) {
      auto id = to_traverse.back();
      SolidTask solid_task;
      solid_task.set_allocated_task_id(new TaskId(id));
      auto found_task = tasks.find(id);
      solid_task.set_allocated_task(new Task(std::move(found_task->second)));
      tasks.erase(found_task);
      if (to_traverse.size() > 1) {
        auto parent_id = to_traverse[to_traverse.size() - 2];
        solid_task.set_allocated_parent_id(new TaskId(std::move(parent_id)));
      }
      result.push_back(std::move(solid_task));
    }

    if (auto it = parents.find(to_traverse.back()); it != parents.end()) {
      if (!it->second.empty()) {
        to_traverse.push_back(it->second.back());
        it->second.pop_back();
        is_growing = true;
      } else {
        parents.erase(it);
      }
      continue;
    }
    is_growing = false;
    to_traverse.pop_back();
    if (!roots.empty() && to_traverse.empty()) {
      to_traverse.push_back(roots.back());
      roots.pop_back();
      is_growing = true;
    }
  }
  return result;
}

OperationResult<MCStatus, SolidTasks> ModelController::GetAllSolidTasks() {
  return OperationResult<Status, SolidTasks>::Ok(
      GetSolidTasksSorted(task_manager_->Show()));
}

OperationResult<MCStatus, SolidTasks> ModelController::GetSpecificSolidTasks(
    std::vector<TaskId> ids) {
  auto storage = task_manager_->Show();
  for (const auto& i : ids) {
    if (storage.tasks.find(i) == storage.tasks.end()) {
      return OperationResult<Status, SolidTasks>::Error(Status::kNotPresentId);
    }
  }
  std::sort(ids.begin(), ids.end(), std::greater<TaskId>());
  storage.roots = std::move(ids);
  return OperationResult<Status, SolidTasks>::Ok(
      GetSolidTasksSorted(std::move(storage)));
}

OperationResult<MCStatus> ModelController::LoadFrom(std::istream& is) {
  Persistence persistence;
  auto result = persistence.Load(is);
  if (!result) {
    return OperationResult<Status>::Error(Status::kLoadFailure);
  }
  TaskManager::Parents parents;
  TaskManager::Tasks tasks;
  TaskManager::Roots roots;
  TaskId max_task_id = result.AccessResult().back().task_id();
  for (auto& i : result.AccessResult()) {
    if (i.task_id() > max_task_id) {
      max_task_id = i.task_id();
    }
    if (!i.has_parent_id()) {
      roots.push_back(i.task_id());
      tasks.insert({i.task_id(), i.task()});
      parents[i.task_id()];
    } else {
      parents[i.parent_id()].push_back(i.task_id());
      tasks.insert({i.task_id(), i.task()});
    }
  }
  TaskManager::Storage storage;
  storage.roots = std::move(roots);
  storage.parents = std::move(parents);
  storage.tasks = std::move(tasks);
  auto task_id_producer =
      std::make_unique<TaskIdProducer>(std::move(max_task_id));
  task_id_producer->GetNextId();
  task_manager_ = std::make_unique<TaskManager>(std::move(task_id_producer),
                                                std::move(storage));
  return OperationResult<Status>::Ok();
}

OperationResult<MCStatus> ModelController::SaveTo(std::ostream& os) {
  auto solid_tasks = GetAllSolidTasks();
  Persistence persistence;
  auto result = persistence.Save(os, std::move(solid_tasks.AccessResult()));
  if (!result) {
    return OperationResult<MCStatus>::Error(Status::kSaveFailure);
  }
  return OperationResult<Status>::Ok();
}
}  // namespace task_manager
