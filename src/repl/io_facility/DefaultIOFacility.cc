#include "DefaultIOFacility.h"

#include <iostream>

inline const char* to_string(Task::Priority priority) {
  switch (priority) {
    case Task::Priority::kLow:
      return "low";
    case Task::Priority::kMedium:
      return "medium";
    case Task::Priority::kHigh:
      return "high";
  }
}

inline const char* to_string(Task::State state) {
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