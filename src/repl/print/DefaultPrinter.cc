#include "DefaultPrinter.h"

#include <iostream>

std::string DefaultPrinter::AskForATitle() { return PrintAndGet("title"); }

std::string DefaultPrinter::AskForADate() { return PrintAndGet("date"); }

std::string DefaultPrinter::AskForAPriority() {
  return PrintAndGet("priority");
}

std::string DefaultPrinter::AskForAConfirmation() { return PrintAndGet("Y/n"); }

std::string DefaultPrinter::AskForAnAction() { return PrintAndGet(""); }

std::string DefaultPrinter::AskForAnId() { return PrintAndGet("id"); }

void DefaultPrinter::ShowHelp() {
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

void DefaultPrinter::Report(const std::string& str) {
  std::cout << str << '\n';
}

void DefaultPrinter::ReportUnknownCommand() {
  Report("Unknown command. Type 'help' to display all available commands.");
}

void DefaultPrinter::ReportNotValidConfirmation() {
  Report("Please, enter yes(y) or no(n).");
}

void DefaultPrinter::ReportNotValidDate() {
  Report("Date should be in format.");  // TODO
}

void DefaultPrinter::ReportNotValidPriority() {
  Report("Priority should be: low, medium, high.");
}

void DefaultPrinter::ReportNotValidId() {
  Report("Id should be numeric value.");
}

void DefaultPrinter::ReportNotValidTitle() {
  Report("Title must not be empty.");
}

void DefaultPrinter::ChangePrompt(std::string prompt) {
  prompt_ = std::move(prompt);
}

std::string DefaultPrinter::PrintAndGet(std::string str) {
  std::cout << prompt_ << '[' << str << ']' << ": ";
  std::string result;
  std::getline(std::cin, result);
  return result;
}