#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMGENERALFUNCTIONAL_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMGENERALFUNCTIONAL_H_

#include <sstream>

#include "model/SolidTask.h"
#include "repl/io_facility/IIoFacility.h"

struct IostreamStrings {
  using ccp = const char*;

  static constexpr ccp kTitleMustNotBeEmpty = "Title must not be empty.\n";
  static constexpr ccp kInvalidState =
      "State should be '+' for completed or '-' for uncompleted\n";
  static constexpr ccp kInvalidPriority =
      "Priority should be: low, medium, high.\n";
  static constexpr ccp kInvalidDate = "Wrong date format.\n";
  static constexpr ccp kAddSubtaskTo = "Add subtask to:\n";
  static constexpr ccp kOkayITreatItAsNo = "Okay, i treat it as no\n";
  static constexpr ccp kYouAreGoingToEdit = "You are going to edit:\n";
  static constexpr ccp kUnknownCommand =
      "Unknown command. Type 'help' to display all available commands.\n";
  static constexpr ccp kInvalidId = "Id should be numeric value.\n";
  static constexpr ccp kMultipleId =
      "Multiple Id does not supported for this command.\n";
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

  static std::string AndItsChildren(const std::string&);

  static std::string LeaveEmptyFor(const std::string&);

  static std::string LeaveEmptyFor(const std::string& str1,
                                   const std::string& str2);

  static std::string GetPrompt(const std::string&);

  static std::string GetPrompt(const std::string&, const std::string&);

  static std::string ProceedTo(const std::string&);

  static const char* to_string(Task::Priority priority);
  static const char* to_string(Task::Progress state);

  static std::string ShowSolidTask(SolidTask);

  static std::string ShowSolidTasks(SolidTasks);
};

std::string PrintAndGet(IIoFacility& io, const std::string& str);

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMGENERALFUNCTIONAL_H_
