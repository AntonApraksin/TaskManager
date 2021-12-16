#ifndef TASKMANAGER_SRC_REPL_CONTROLLER_H_
#define TASKMANAGER_SRC_REPL_CONTROLLER_H_

#include "model/task_manager/TaskManager.h"
#include "repl/view/View.h"
#include "repl/view/steps/IStepFactory.h"

class Controller {
 public:
  Controller(std::unique_ptr<View> view, std::shared_ptr<TaskManager>,
             std::shared_ptr<IValidator> validator,
             std::unique_ptr<IStepFactory>);

  void Run();

 private:
  void PerformAction(CommandEnum, std::string);

  void HandleAdd(std::string);
  void HandleAdd();
  void HandleAdd(TaskId);

  void HandleEdit(std::string);
  void HandleEdit(TaskId);

  void HandleComplete(std::string);
  void HandleComplete(std::vector<TaskId>);

  void HandleDelete(std::string);
  void HandleDelete(std::vector<TaskId>);

  void HandleShow(std::string);
  void HandleShow(std::vector<TaskId>);
  void HandleShow();

  void HandleHelp(std::string);
  void HandleHelp();

  void HandleUnknown(std::string);
  void HandleUnknown();

  void ReportMessage(MessageEnum);
  void ReportMessage(MessageEnum, std::string);

  std::optional<Task> GetTaskById(const TaskStorage&, TaskId);
  std::optional<TaskWrapper> GetTaskWrapperById(const TaskStorage&, TaskId);

 private:
  std::shared_ptr<TaskManager> task_manager_;
  std::unique_ptr<View> view_;
  std::shared_ptr<IValidator> validator_;

  std::unique_ptr<IStepFactory> step_factory_;
};

#endif  // TASKMANAGER_SRC_REPL_CONTROLLER_H_
