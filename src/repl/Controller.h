#ifndef TASKMANAGER_SRC_REPL_CONTROLLER_H_
#define TASKMANAGER_SRC_REPL_CONTROLLER_H_

#include "model/task_manager/TaskManager.h"
#include "repl/view/View.h"
#include "repl/view/step/IStepFactory.h"

class Controller {
 public:
  Controller(const std::shared_ptr<IValidator>&,
             const std::shared_ptr<TaskManager>&,
             std::unique_ptr<IStepFactory>);

  void Run();

 private:
  void PerformAction(StateEnum, const std::vector<TaskId>& ids);
  void HandleAdd();
  void HandleAdd(TaskId);
  void HandleEdit(TaskId);
  void HandleComplete(TaskId);
  void HandleDelete(TaskId);
  void HandleShow();
  void HandleHelp();

  void ShowByMap(const std::map<TaskId, TaskWrapper>&);

 private:
  std::shared_ptr<TaskManager> task_manager_;
  std::unique_ptr<View> view_;

  std::unique_ptr<IStepFactory> step_factory_;
};

#endif  // TASKMANAGER_SRC_REPL_CONTROLLER_H_
