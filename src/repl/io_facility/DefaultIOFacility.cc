#include "DefaultIOFacility.h"

#include <iostream>

std::string DefaultIOFacility::AskForATitle() { return PrintAndGet("title"); }

std::string DefaultIOFacility::AskForADate() { return PrintAndGet("date"); }

std::string DefaultIOFacility::AskForAPriority() {
  return PrintAndGet("priority");
}

std::string DefaultIOFacility::AskForAConfirmation() { return PrintAndGet("Y/n"); }

std::string DefaultIOFacility::AskForAnAction() { return PrintAndGet(""); }

std::string DefaultIOFacility::AskForAnId() { return PrintAndGet("id"); }

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