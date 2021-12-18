#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEP_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEP_H_

#include <vector>

#include "StepResult.h"
#include "model/SolidTask.h"
#include "repl/view/steps/MessageEnum.h"

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
  using OptionalArg = std::optional<std::string>;

  void SetArg(std::string arg) { arg_ = std::move(arg); }

  void SetMessage(MessageEnum main_error) { main_error_ = main_error; }

 protected:
  OptionalArg arg_;
  MessageEnum main_error_;
};
#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_ISTEP_H_