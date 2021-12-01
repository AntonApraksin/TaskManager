#ifndef TASKMANAGER_SRC_REPL_PRINT_IPRINTER_H_
#define TASKMANAGER_SRC_REPL_PRINT_IPRINTER_H_

#include <string>

#include "model/id/TaskId.h"
#include "model/task/Task.h"

class IIOFacility {
 public:
  virtual ~IIOFacility() {}

  virtual std::string AskForATitle() = 0;
  virtual std::string AskForADate() = 0;
  virtual std::string AskForAPriority() = 0;
  virtual std::string AskForAConfirmation() = 0;
  virtual std::string AskForAnAction() = 0;

  virtual void ShowHelp() = 0;

  virtual void ShowId(TaskId) = 0;
  virtual void ShowTask(Task) = 0;
  virtual void ShowTaskAndId(TaskId, Task) = 0;

  virtual void ReportUnknownCommand() = 0;
  virtual void ReportNotValidTitle() = 0;
  virtual void ReportNotValidDate() = 0;
  virtual void ReportNotValidPriority() = 0;
  virtual void ReportNotValidId() = 0;
  virtual void ReportNotPresentId(TaskId) = 0;
  virtual void ReportMultipleId() = 0;
  virtual void ReportRequiredId() = 0;
  virtual void ReportNotValidConfirmation() = 0;

  virtual void ChangePrompt(std::string) = 0;

 protected:
  std::string prompt_;
};

#endif  // TASKMANAGER_SRC_REPL_PRINT_IPRINTER_H_
