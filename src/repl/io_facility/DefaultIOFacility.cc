#include "DefaultIOFacility.h"

#include <iostream>

#include "repl/io_facility/DateFormat.h"

std::string DefaultIOFacility::AskForATitle() { return PrintAndGet("title"); }

std::string DefaultIOFacility::AskForADate() {
  std::cout << prompt_ << "[date(" << kDatePattern << ")]: ";
  std::string result;
  std::getline(std::cin, result);
  return result;
}

std::string DefaultIOFacility::AskForAPriority() {
  return PrintAndGet("priority");
}

void DefaultIOFacility::Report(const std::string& str) {
  std::cout << str << '\n';
}

void DefaultIOFacility::ReportNotValidDate() {
  Report("Date should be in format.");  // TODO
}

void DefaultIOFacility::ReportNotValidPriority() {
  Report("Priority should be: low, medium, high.");
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