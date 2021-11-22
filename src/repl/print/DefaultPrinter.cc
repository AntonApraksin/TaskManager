#include "DefaultPrinter.h"

#include <iostream>

std::string DefaultPrinter::AskForATitle() {
  return PrintAndGet("title");
}

std::string DefaultPrinter::AskForADate() {
  return PrintAndGet("date");
}

std::string DefaultPrinter::AskForAPriority() {
  return PrintAndGet("priority");
}

std::string DefaultPrinter::AskForAConfirmation() {
  return PrintAndGet("Y/n");
}

std::string DefaultPrinter::AskForAnAction() {
  return PrintAndGet("");
}

void DefaultPrinter::ShowHelp() {
  std::cout << "This is very helpful help\n";
}

void DefaultPrinter::ChangePrompt(std::string prompt) {
  prompt_ = std::move(prompt);
}

std::string DefaultPrinter::PrintAndGet(std::string str) {
  std::cout << prompt_ << '[' << str << ']' << ": ";
  std::string result;
  std::cin >> result;
  return result;
}