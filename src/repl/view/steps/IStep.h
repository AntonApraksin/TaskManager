#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEP_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEP_H_

#include <vector>

#include "StepResult.h"
#include "model/task_wrapper/TaskWrapper.h"

class IStep {
 public:
  virtual StepResult Run() = 0;
  virtual ~IStep() {}
};

class IAddTaskStep : public IStep {
 public:
  void SetTask(Task task) { task_ = task; }

  void ResetTask() { task_.reset(); }

 protected:
  std::optional<Task> task_;
};

class IEditTaskStep : public IStep {
 public:
  using TaskWrapperRef = std::reference_wrapper<const TaskWrapper>;

  explicit IEditTaskStep(TaskWrapperRef task_wrapper)
      : task_wrapper_(task_wrapper) {}

  void SetTaskWrapper(TaskWrapperRef task_wrapper) {
    task_wrapper_ = task_wrapper;
  }

 protected:
  TaskWrapperRef task_wrapper_;
};

class IDeleteTaskStep : public IStep {
 public:
  using TaskWrappers = std::vector<std::reference_wrapper<const TaskWrapper>>;

  explicit IDeleteTaskStep(TaskWrappers task_wrappers)
      : task_wrappers_(std::move(task_wrappers)) {}

  void SetTaskWrappers(TaskWrappers task_wrappers) {
    task_wrappers_ = std::move(task_wrappers);
  }

 protected:
  TaskWrappers task_wrappers_;
};

class ICompleteTaskStep : public IStep {
 public:
  using TaskWrappers = std::vector<std::reference_wrapper<const TaskWrapper>>;

  explicit ICompleteTaskStep(TaskWrappers task_wrappers)
      : task_wrappers_(std::move(task_wrappers)) {}

  void SetTaskWrappers(TaskWrappers task_wrappers) {
    task_wrappers_ = std::move(task_wrappers);
  }

 protected:
  TaskWrappers task_wrappers_;
};

class IShowAllTasksStep : public IStep {
 public:
  using TaskStorageRef = std::reference_wrapper<const TaskStorage>;

  explicit IShowAllTasksStep(TaskStorageRef task_storage)
      : task_storage_(task_storage) {}

  void SetTaskStorage(TaskStorageRef task_storage) {
    task_storage_ = task_storage;
  }

 protected:
  TaskStorageRef task_storage_;
};

class IShowNTasksStep : public IStep {
 public:
  using TaskWrappers = std::vector<std::reference_wrapper<const TaskWrapper>>;

  explicit IShowNTasksStep(TaskWrappers task_wrappers)
      : task_wrappers_(task_wrappers) {}

  void SetTaskWrappers(TaskWrappers task_wrappers) {
    task_wrappers_ = task_wrappers;
  }

 protected:
  TaskWrappers task_wrappers_;
};

class IShowSortedTasksStep : public IStep {
 public:
  using Tasks = std::vector<std::pair<TaskId, Task>>;

  explicit IShowSortedTasksStep(Tasks tasks) : tasks_(tasks) {}

  void SetTasks(Tasks tasks) { tasks_ = tasks; }

 protected:
  Tasks tasks_;
};
#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEP_H_