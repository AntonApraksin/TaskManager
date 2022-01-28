#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMGENERALFUNCTIONAL_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMGENERALFUNCTIONAL_H_

#include <sstream>

#include "interactor/io_facility/IIoFacility.h"
#include "model/SolidTask.h"

namespace task_manager {
namespace Strings {  // NOLINT: For a purpose
using ccp = const char*;

constexpr ccp kTitleMustNotBeEmpty = "Title must not be empty.\n";
constexpr ccp kStateShouldBe =
    "State should be '+' for completed or '-' for uncompleted\n";
constexpr ccp kPriorityShouldBe = "Priority should be: low, medium, high.\n";
constexpr ccp kWrongDateFormat = "Wrong date format.\n";
constexpr ccp kAddSubtaskTo = "Add subtask to:\n";
constexpr ccp kOkayITreatItAsNo = "Okay, i treat it as no\n";
constexpr ccp kYouAreGoingToEdit = "You are going to edit:\n";
constexpr ccp kUnknownCommand =
    "Unknown command. Type 'help' to display all available commands.\n";
constexpr ccp kMultipleArgumentDoesNotSupported =
    "Multiple arguments does not supported for this command.\n";
constexpr ccp kRequiredId = "Id is required for this command.\n";
constexpr ccp kRepeatedId = "Repeated Ids are not allowed.\n";
constexpr ccp kNotPresentId = "Given Id wasn`t found\n";
constexpr ccp kHelp =
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
    "  save(sa)\n"
    "    Save current state.\n"
    "  load(lo)\n"
    "    Load state. All current state is erased.\n"
    "  quit(q)\n"
    "    Quit.\n";

constexpr ccp kLoadFailure = "Failure during load\n";
constexpr ccp kSaveFailure = "Failure during save\n";

std::string NotPresentId(const std::string&);

std::string InvalidId(const std::string&);

std::string ShowId(const std::string&);

std::string YouAreGoingTo(const std::string&);

std::string LeaveEmptyFor(const std::string&);

std::string LeaveEmptyFor(const std::string&, const std::string&);

std::string GetPrompt(const std::string&);

std::string GetPrompt(const std::string&, const std::string&);

std::string ProceedTo(const std::string&);

std::string ShowSolidTask(SolidTask);

std::string ShowSolidTasks(SolidTasks);

std::string IdIsSubIdOf(const std::string&, const std::string&);

std::string FilenameDoesNotExist(const std::string&);
std::string ErrorDuringOpeningFile(const std::string&);
std::string FailureDuringSaving(const std::string&);
std::string FailureDuringLoading(const std::string&);

const char* to_string(Task::Priority);
const char* to_string(Task::Progress);
}  // namespace Strings

std::string PrintAndGet(IIoFacility&, const std::string&);
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMGENERALFUNCTIONAL_H_
