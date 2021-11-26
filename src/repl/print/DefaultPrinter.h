#ifndef TASKMANAGER_SRC_REPL_PRINT_DEFAULTPRINTER_H_
#define TASKMANAGER_SRC_REPL_PRINT_DEFAULTPRINTER_H_

#include "repl/print/IPrinter.h"

class DefaultPrinter : public IPrinter {
 public:
  std::string AskForATitle() override;
  std::string AskForADate() override;
  std::string AskForAPriority() override;
  std::string AskForAConfirmation() override;
  std::string AskForAnAction() override;
  std::string AskForAnId() override;

  void ReportUnknownCommand() override;
  void ReportNotValidTitle() override;
  void ReportNotValidDate() override;
  void ReportNotValidPriority() override;
  void ReportNotValidId() override;
  void ReportNotValidConfirmation() override;

  void ShowHelp() override;

  void ChangePrompt(std::string) override;

 private:
  std::string PrintAndGet(std::string);
  void Report(const std::string&);
};

#endif  // TASKMANAGER_SRC_REPL_PRINT_DEFAULTPRINTER_H_
