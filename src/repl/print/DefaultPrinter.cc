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
            << "  add\n"
            << "    Add a new task.\n"
            << "  edit id\n"
            << "    Edit a task with given id.\n"
            << "  delete id\n"
            << "    Delete a task with the given id.\n"
            << "  complete id\n"
            << "    Complete a task with the given id.\n"
            << "  show\n"
            << "    Show all uncompleted tasks.\n";
}

void DefaultPrinter::ReportUnknownCommand() {
  std::cout
      << "Unknown command. Type 'help' to display all available commands.\n";
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