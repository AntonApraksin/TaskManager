#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEP_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEP_H_

#include <vector>

#include "model/task_wrapper/TaskWrapper.h"
#include "repl/view/step/StepResult.h"

class IStep {
 public:
  virtual StepResult Run() = 0;
  virtual ~IStep() {}
};

class IAddTaskREPLState : public IStep {
 public:
  void SetTask(Task task) { task_ = task; }

  void ResetTask() { task_.reset(); }

 protected:
  std::optional<Task> task_;
};

class IEditTaskREPLState : public IStep {
 public:
  using TaskWrapperRef = std::reference_wrapper<TaskWrapper>;

  IEditTaskREPLState(TaskWrapperRef task_wrapper)
      : task_wrapper_(task_wrapper) {}

  void SetTaskWrapper(TaskWrapperRef task_wrapper) {
    task_wrapper_ = task_wrapper;
  }

 protected:
  TaskWrapperRef task_wrapper_;
};

class IDeleteTaskREPLState : public IStep {
 public:
  using TaskWrappers = std::vector<std::reference_wrapper<TaskWrapper>>;

  IDeleteTaskREPLState(TaskWrappers task_wrappers)
      : task_wrappers_(std::move(task_wrappers)) {}

  void SetTaskWrappers(TaskWrappers task_wrappers) {
    task_wrappers_ = std::move(task_wrappers);
  }

 protected:
  TaskWrappers task_wrappers_;
};

class ICompleteTaskREPLState : public IStep {
 public:
  using TaskWrappers = std::vector<std::reference_wrapper<TaskWrapper>>;

  ICompleteTaskREPLState(TaskWrappers task_wrappers)
      : task_wrappers_(std::move(task_wrappers)) {}

  void SetTaskWrappers(TaskWrappers task_wrappers) {
    task_wrappers_ = std::move(task_wrappers);
  }

 protected:
  TaskWrappers task_wrappers_;
};

class IShowAllTasksREPLState : public IStep {
 public:
  using TaskStorageRef = std::reference_wrapper<TaskStorage>;

  IShowAllTasksREPLState(TaskStorageRef task_storage)
      : task_storage_(task_storage) {}

  void SetTaskStorage(TaskStorageRef task_storage) {
    task_storage_ = task_storage;
  }

 protected:
  TaskStorageRef task_storage_;
};

class IShowNTasksREPLState : public IStep {
 public:
  using TaskWrappers = std::vector<std::reference_wrapper<TaskWrapper>>;

  IShowNTasksREPLState(TaskWrappers task_wrappers)
      : task_wrappers_(task_wrappers) {}

  void SetTaskWrappers(TaskWrappers task_wrappers) {
    task_wrappers_ = task_wrappers;
  }

 protected:
  TaskWrappers task_wrappers_;
};

class IShowSortedTasksREPLState : public IStep {
 public:
  using Tasks = std::vector<std::pair<TaskId, Task>>;

  IShowSortedTasksREPLState(Tasks tasks) : tasks_(tasks) {}

  void SetTasks(Tasks tasks) { tasks_ = tasks; }

 protected:
  Tasks tasks_;
};

class IUnknownCommandsREPLState : public IStep {};

class IShowHelpREPLState : public IStep {};

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEP_H_