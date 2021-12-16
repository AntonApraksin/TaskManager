#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGER_TASKMANAGER_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGER_TASKMANAGER_H_
#pragma once

#include <map>
#include <memory>

#include "model/id/TaskIdProducer.h"
#include "model/task/Task.h"
#include "model/task_wrapper/TaskWrapper.h"

class TaskManager final {
 public:
  struct ActionResult {
    enum class Status {
      kOk,
      kNotPresentId,
      kFailure,
    };
    Status status;

    static ActionResult error(Status status) { return ActionResult{status}; }

    static ActionResult ok() { return ActionResult{Status::kOk}; }
  };

  explicit TaskManager(std::unique_ptr<ITaskIdProducer> id_producer);

  std::pair<std::optional<TaskId>, ActionResult> Add(Task task);
  std::pair<std::optional<TaskId>, ActionResult> Add(TaskId task_id, Task task);

  ActionResult Edit(TaskId id, Task task);
  ActionResult Complete(TaskId id);
  ActionResult Delete(TaskId id);

  TaskStorage Show() const;

  TaskId GetLastGivenId() const;

 private:
  TaskStorage storage_;
  std::unique_ptr<ITaskIdProducer> id_producer_;
};

#endif  // TASKMANAGER_SRC_MODEL_TASKMANAGER_TASKMANAGER_H_
