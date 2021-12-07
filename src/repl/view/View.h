#ifndef TASKMANAGER_SRC_REPL_VIEW_VIEW_H_
#define TASKMANAGER_SRC_REPL_VIEW_VIEW_H_

#include <vector>

#include "model/task/Task.h"
#include "repl/io_facility/IIoFacility.h"
#include "repl/validator/IValidator.h"
#include "repl/view/StateEnum.h"
#include "repl/view/steps/ISmallStepFactory.h"
#include "repl/view/steps/IStep.h"

class View {
 public:
  explicit View(const std::shared_ptr<IIoFacility>& io_facility,
                const std::shared_ptr<IValidator>& validator);

  void SetState(const std::shared_ptr<IStep>&);
  StepResult Run();

  void ShowHelp();
  void ReportUnknownCommand();
  void ReportNotPresentId(TaskId);
  void ReportInvalidId();
  void ReportMultipleId();
  void ReportRequiredId();

  void ShowId(TaskId);

  std::pair<StateEnum, std::vector<TaskId>> GetNextCommand();

 private:
  std::shared_ptr<IValidator> validator_;
  std::shared_ptr<IIoFacility> io_facility_;
  std::shared_ptr<IStep> current_step_;
};

#endif  // TASKMANAGER_SRC_REPL_VIEW_VIEW_H_