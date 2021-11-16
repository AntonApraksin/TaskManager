#ifndef TASK_MANAGER_INCLUDE_MODEL_TASK_H_
#define TASK_MANAGER_INCLUDE_MODEL_TASK_H_
#pragma once

#include <ctime>
#include <string>

class Task {
 public:
  enum class Priority {
    kHigh,
    kMedium,
    kLow,
    kNone,
  };

  using Date_t = std::chrono::time_point<std::chrono::system_clock>;

 public:
  static Task Create(std::string title, Priority priority, Date_t due_date);

 public:
  std::string GetTitle() const;
  Priority GetPriority() const;
  Date_t GetDueDate() const;

 private:
  Task(std::string title, Priority priority, Date_t due_date);

 private:
  std::string title_;
  Priority priority_;
  Date_t due_date_;
};

#endif  // TASK_MANAGER_INCLUDE_MODEL_TASK_H_
