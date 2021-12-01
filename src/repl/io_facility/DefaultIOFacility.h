#ifndef TASKMANAGER_SRC_REPL_PRINT_DEFAULTPRINTER_H_
#define TASKMANAGER_SRC_REPL_PRINT_DEFAULTPRINTER_H_

#include "repl/io_facility/IIOFacility.h"

class DefaultIOFacility : public IIOFacility {
 public:
  std::string AskForATitle() override;
  std::string AskForADate() override;
  std::string AskForAPriority() override;
  std::string AskForAConfirmation() override;
  std::string AskForAnAction() override;

  void ReportUnknownCommand() override;
  void ReportNotValidTitle() override;
  void ReportNotValidDate() override;
  void ReportNotValidPriority() override;
  void ReportNotValidId() override;
  void ReportNotPresentId(TaskId) override;
  void ReportMultipleId() override;
  void ReportRequiredId() override;
  void ReportNotValidConfirmation() override;

  void ShowHelp() override;

  void ShowId(TaskId) override;
  void ShowTask(Task) override;
  void ShowTaskAndId(TaskId, Task) override;

  void ChangePrompt(std::string) override;

 private:
  std::string PrintAndGet(std::string);
  void Report(const std::string&);
};

#endif  // TASKMANAGER_SRC_REPL_PRINT_DEFAULTPRINTER_H_
