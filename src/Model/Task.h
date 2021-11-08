#ifndef TASK_MANAGER_SRC_MODEL_TASK_H_
#define TASK_MANAGER_SRC_MODEL_TASK_H_
#pragma once

#include <ctime>
#include <string>

class Task {
 public:
  enum class Priority {
    HIGH,
    MEDIUM,
    LOW,
    NONE,
  };

  using Date_t = std::chrono::time_point<std::chrono::system_clock>;

  static Task Create(std::string title, Priority priority, Date_t due_date) {
    return {std::move(title), priority, std::move(due_date)};
  }

  std::string GetTitle() const { return title_; }

  Priority GetPriority() const { return priority_; }

  Date_t GetDueDate() const { return due_date_; }

 private:
  Task(std::string title, Priority priority, Date_t due_date)
      : title_(std::move(title)),
        priority_(priority),
        due_date_(std::move(due_date)) {}

 private:
  std::string title_;
  Priority priority_;
  Date_t due_date_;
};

#endif  // TASK_MANAGER_SRC_MODEL_TASK_H_
