#ifndef TASKMANAGER_SRC_REPL_CONTROLLER_H_
#define TASKMANAGER_SRC_REPL_CONTROLLER_H_

#include "model/task_manager/TaskManager.h"
#include "repl/view/View.h"
#include "repl/view/steps/IStepFactory.h"

class Controller {
 public:
  Controller(std::unique_ptr<View> view, const std::shared_ptr<TaskManager>&,
             std::unique_ptr<IStepFactory>);

  void Run();

 private:
  void PerformAction(CommandEnum, const std::vector<TaskId>& ids);
  void HandleAdd();
  void HandleAdd(TaskId);
  void HandleEdit(TaskId);
  void HandleComplete(const std::vector<TaskId>&);
  void HandleDelete(const std::vector<TaskId>&);
  void HandleShow();
  void HandleHelp();

  void ReportMessage(MessageEnum);
  void ReportMessage(MessageEnum, TaskId);

 private:
  std::shared_ptr<TaskManager> task_manager_;
  std::unique_ptr<View> view_;

  std::unique_ptr<IStepFactory> step_factory_;
};

#endif  // TASKMANAGER_SRC_REPL_CONTROLLER_H_
