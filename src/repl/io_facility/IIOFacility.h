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

  virtual void ReportNotValidTitle() = 0;
  virtual void ReportNotValidDate() = 0;
  virtual void ReportNotValidPriority() = 0;

  virtual void ChangePrompt(std::string) = 0;

 protected:
  std::string prompt_;
};

#endif  // TASKMANAGER_SRC_REPL_PRINT_IPRINTER_H_
