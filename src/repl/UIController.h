#ifndef TASKMANAGER_SRC_REPL_UICONTROLLER_H_
#define TASKMANAGER_SRC_REPL_UICONTROLLER_H_

#include "model/ModelController.h"
#include "repl/view/View.h"
#include "repl/view/steps/IStepFactory.h"

class UIController {
 public:
  UIController(std::unique_ptr<View>, std::shared_ptr<ModelController>,
               std::shared_ptr<IValidator>, std::unique_ptr<IStepFactory>);

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

 private:
  std::shared_ptr<ModelController> model_controller_;
  std::unique_ptr<View> view_;
  std::shared_ptr<IValidator> validator_;

  std::unique_ptr<IStepFactory> step_factory_;
};

#endif  // TASKMANAGER_SRC_REPL_UICONTROLLER_H_
