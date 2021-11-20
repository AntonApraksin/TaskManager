#include "Task.h"

std::optional<Task> Task::Create(std::string title, Priority priority,
                                 Date_t due_date, State state) {
  if (title.empty()) {
    return std::nullopt;
  }
  return Task{std::move(title), priority, std::move(due_date), state};
}

std::string Task::GetTitle() const { return title_; }

Task::Priority Task::GetPriority() const { return priority_; }

Date_t Task::GetDueDate() const { return due_date_; }

Task::State Task::GetState() const { return state_; }

Task::Task(std::string title, Priority priority, Date_t due_date, State state)
    : title_(std::move(title)),
      priority_(priority),
      due_date_(std::move(due_date)),
      state_(state) {}
