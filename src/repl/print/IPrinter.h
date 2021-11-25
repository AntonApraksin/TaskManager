#ifndef TASKMANAGER_SRC_REPL_PRINT_IPRINTER_H_
#define TASKMANAGER_SRC_REPL_PRINT_IPRINTER_H_

#include <string>

class IPrinter {
 public:
  virtual ~IPrinter() {}

  virtual std::string AskForATitle() = 0;
  virtual std::string AskForADate() = 0;
  virtual std::string AskForAPriority() = 0;
  virtual std::string AskForAConfirmation() = 0;
  virtual std::string AskForAnAction() = 0;
  virtual std::string AskForAnId() = 0;

  virtual void ShowHelp() = 0;
  virtual void ReportUnknownCommand() = 0;

  virtual void ChangePrompt(std::string) = 0;

 protected:
  std::string prompt_;
};

#endif  // TASKMANAGER_SRC_REPL_PRINT_IPRINTER_H_
