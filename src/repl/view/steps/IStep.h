#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEP_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEP_H_

#include <vector>

#include "model/SolidTask.h"
#include "repl/view/steps/StepResult.h"

namespace task_manager {
class IStep {
 public:
  virtual StepResult Run() = 0;
  virtual ~IStep() {}
};

class IAddTaskStep : public IStep {
 public:
  void SetSolidTask(SolidTask solid_task) {
    solid_task_ = std::move(solid_task);
  }

 protected:
  std::optional<task_manager::SolidTask> solid_task_;
};

class IEditTaskStep : public IStep {
 public:
  void SetSolidTask(SolidTask solid_task) {
    solid_task_ = std::move(solid_task);
  }

 protected:
  SolidTask solid_task_;
};

class StepWithSolidTasks {
 public:
  void SetSolidTasks(SolidTasks solid_tasks) {
    solid_tasks_ = std::move(solid_tasks);
  }

 protected:
  ~StepWithSolidTasks() = default;

 protected:
  SolidTasks solid_tasks_;
};

class IDeleteTaskStep : public StepWithSolidTasks, public IStep {};

class ICompleteTaskStep : public StepWithSolidTasks, public IStep {};

class IShowStep : public StepWithSolidTasks, public IStep {};

class IShowHelpStep : public IStep {};

class IReportMessageStep : public IStep {
 public:
  void SetMessage(std::string message) { message_ = std::move(message); }

 protected:
  std::string message_;
};
}  // namespace task_manager
#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEP_H_