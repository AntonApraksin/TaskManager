#include "Model/task.h"

Task Task::Create(std::string title, Priority priority, Date_t due_date) {
  return {std::move(title), priority, std::move(due_date)};
}

std::string Task::GetTitle() const { return title_; }

Task::Priority Task::GetPriority() const { return priority_; }

Task::Date_t Task::GetDueDate() const { return due_date_; }

Task::Task(std::string title, Priority priority, Date_t due_date)
    : title_(std::move(title)),
      priority_(priority),
      due_date_(std::move(due_date)) {}