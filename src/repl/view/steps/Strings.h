#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMGENERALFUNCTIONAL_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMGENERALFUNCTIONAL_H_

#include <sstream>

#include "model/SolidTask.h"
#include "repl/io_facility/IIoFacility.h"

namespace task_manager {
struct Strings {
  using ccp = const char*;

  static constexpr ccp kTitleMustNotBeEmpty = "Title must not be empty.\n";
  static constexpr ccp kStateShouldBe =
      "State should be '+' for completed or '-' for uncompleted\n";
  static constexpr ccp kPriorityShouldBe =
      "Priority should be: low, medium, high.\n";
  static constexpr ccp kWrongDateFormat = "Wrong date format.\n";
  static constexpr ccp kAddSubtaskTo = "Add subtask to:\n";
  static constexpr ccp kOkayITreatItAsNo = "Okay, i treat it as no\n";
  static constexpr ccp kYouAreGoingToEdit = "You are going to edit:\n";
  static constexpr ccp kUnknownCommand =
      "Unknown command. Type 'help' to display all available commands.\n";
  static constexpr ccp kMultipleArgumentDoesNotSupported =
      "Multiple arguments does not supported for this command.\n";
  static constexpr ccp kRequiredId = "Id is required for this command.\n";
  static constexpr ccp kRepeatedId = "Repeated Ids are not allowed.\n";
  static constexpr ccp kHelp =
      "Usage:\n"
      "  add(a) [id]\n"
      "    Add a new task.\n"
      "  edit(e) id\n"
      "    Edit a task with given id.\n"
      "  delete(d) id [id...]\n"
      "    Delete a task with the given id.\n"
      "  complete(c) id [id...]\n"
      "    Complete a task with the given id.\n"
      "  show(s)\n"
      "    Show all tasks.\n"
      "  quit(q)\n"
      "    Quit.\n";

  static std::string NotPresentId(const std::string&);

  static std::string InvalidId(const std::string&);

  static std::string ShowId(const std::string&);

  static std::string YouAreGoingTo(const std::string&);

  static std::string LeaveEmptyFor(const std::string&);

  static std::string LeaveEmptyFor(const std::string&, const std::string&);

  static std::string GetPrompt(const std::string&);

  static std::string GetPrompt(const std::string&, const std::string&);

  static std::string ProceedTo(const std::string&);

  static std::string ShowSolidTask(SolidTask);

  static std::string ShowSolidTasks(SolidTasks);

  static std::string IdIsSubIdOf(const std::string&, const std::string&);

  static std::string FilenameDoesNotExist(const std::string&);
  static std::string ErrorDuringOpeningFile(const std::string&);
  static std::string FailureDuringSaving(const std::string&);
  static std::string FailureDuringLoading(const std::string&);

  static const char* to_string(Task::Priority);
  static const char* to_string(Task::Progress);
};

std::string PrintAndGet(IIoFacility&, const std::string&);
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMGENERALFUNCTIONAL_H_
