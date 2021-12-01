#include "DefaultIOFacility.h"

#include <iostream>

const char* to_string(Task::Priority priority) {
  switch (priority) {
    case Task::Priority::kLow:
      return "low";
    case Task::Priority::kMedium:
      return "medium";
    case Task::Priority::kHigh:
      return "high";
  }
}

const char* to_string(Task::State state) {
  switch (state) {
    case Task::State::kCompleted:
      return "+";
    case Task::State::kUncompleted:
      return "-";
  }
}

std::string DefaultIOFacility::AskForATitle() { return PrintAndGet("title"); }

std::string DefaultIOFacility::AskForADate() { return PrintAndGet("date"); }

std::string DefaultIOFacility::AskForAPriority() {
  return PrintAndGet("priority");
}

std::string DefaultIOFacility::AskForAConfirmation() {
  return PrintAndGet("Y/n");
}

std::string DefaultIOFacility::AskForAnAction() { return PrintAndGet(""); }

void DefaultIOFacility::ShowHelp() {
  std::cout << "Usage:\n"
            << "  add(a)\n"
            << "    Add a new task.\n"
            << "  edit(ed) id\n"
            << "    Edit a task with given id.\n"
            << "  delete(d) id\n"
            << "    Delete a task with the given id.\n"
            << "  complete(c) id\n"
            << "    Complete a task with the given id.\n"
            << "  show(s)\n"
            << "    Show all uncompleted tasks.\n"
            << "  exit(ex)\n"
            << "    Exit.\n";
}

void DefaultIOFacility::Report(const std::string& str) {
  std::cout << str << '\n';
}

void DefaultIOFacility::ReportUnknownCommand() {
  Report("Unknown command. Type 'help' to display all available commands.");
}

void DefaultIOFacility::ReportNotValidConfirmation() {
  Report("Please, enter yes(y) or no(n).");
}

void DefaultIOFacility::ReportNotValidDate() {
  Report("Date should be in format.");  // TODO
}

void DefaultIOFacility::ReportNotValidPriority() {
  Report("Priority should be: low, medium, high.");
}

void DefaultIOFacility::ReportNotValidId() {
  Report("Id should be numeric value.");
}

void DefaultIOFacility::ReportNotPresentId(TaskId task_id) {
  std::cout << task_id.GetId() << " not present in store.\n";
}

void DefaultIOFacility::ReportMultipleId() {
  Report("Multiple Id does not supported for this command.");
}

void DefaultIOFacility::ReportRequiredId() {
  Report("Id is required for this command.");
}

void DefaultIOFacility::ReportNotValidTitle() {
  Report("Title must not be empty.");
}

void DefaultIOFacility::ChangePrompt(std::string prompt) {
  prompt_ = std::move(prompt);
}

std::string DefaultIOFacility::PrintAndGet(std::string str) {
  std::cout << prompt_ << '[' << str << ']' << ": ";
  std::string result;
  std::getline(std::cin, result);
  return result;
}
void DefaultIOFacility::ShowId(TaskId task_id) {
  std::cout << task_id.GetId() << '\n';
}

void DefaultIOFacility::ShowTask(Task task) {
  std::cout << to_string(task.GetState()) << ' ' << task.GetTitle() << '('
            << to_string(task.GetPriority()) << ")\n";  // TODO: add date
}

void DefaultIOFacility::ShowTaskAndId(TaskId task_id, Task task) {
  std::cout << " - " << task_id.GetId() << ": [" << to_string(task.GetState())
            << "] " << task.GetTitle() << '(' << to_string(task.GetPriority())
            << ")\n";  // TODO: add date
}
