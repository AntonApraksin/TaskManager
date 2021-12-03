#ifndef TASKMANAGER_SRC_REPL_VIEW_VIEW_H_
#define TASKMANAGER_SRC_REPL_VIEW_VIEW_H_

#include <vector>

#include "model/task/Task.h"
#include "repl/io_facility/IIOFacility.h"
#include "repl/io_facility/IValidator.h"
#include "repl/view/StateEnum.h"
#include "repl/view/small_step/ISmallStepFactory.h"
#include "repl/view/step/IStep.h"

class View {
 public:
  View(const std::shared_ptr<IValidator>& validator);

  void SetState(const std::shared_ptr<IStep>&);
  StepResult Run();

  void ShowHelp();
  void ReportUnknownCommand();
  void ReportNotPresentId(TaskId);
  void ReportInvalidId();
  void ReportMultipleId();
  void ReportRequiredId();

  std::pair<StateEnum, std::vector<TaskId>> GetNextCommand();

 private:
  std::shared_ptr<IValidator> validator_;

  std::shared_ptr<IStep> current_step_;
};

#endif  // TASKMANAGER_SRC_REPL_VIEW_VIEW_H_