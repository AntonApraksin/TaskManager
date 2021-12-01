#ifndef TASKMANAGER_SRC_REPL_VIEW_VIEW_H_
#define TASKMANAGER_SRC_REPL_VIEW_VIEW_H_

#include <vector>

#include "model/task/Task.h"
#include "repl/io_facility/IIOFacility.h"
#include "repl/io_facility/IValidator.h"
#include "repl/state_factory/IStateFactory.h"
#include "repl/view/StateEnum.h"

class View {
 public:
  View(const std::shared_ptr<IIOFacility>& io_facility,
       const std::shared_ptr<IValidator>& validator,
       const std::shared_ptr<IStateFactory>& state_factory);

  Task GetTask();
  ConfirmationResult GetConfirmation();
  void ShowTask(Task);
  void ShowId(TaskId);
  void ShowTaskWithId(TaskId, Task);

  void SetState(StateEnum);

  void ShowHelp();
  void ReportUnknownCommand();
  void ReportNotPresentId(TaskId);
  void ReportInvalidId();
  void ReportMultipleId();
  void ReportRequiredId();

  std::pair<StateEnum, std::vector<TaskId>> GetNextCommand();

 private:
  std::shared_ptr<IIOFacility> io_facility_;
  std::shared_ptr<IValidator> validator_;
  std::shared_ptr<IStateFactory> state_factory_;
};

#endif  // TASKMANAGER_SRC_REPL_VIEW_VIEW_H_