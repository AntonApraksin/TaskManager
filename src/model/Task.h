#ifndef TASKMANAGER_SRC_MODEL_TASK_H_
#define TASKMANAGER_SRC_MODEL_TASK_H_
#pragma once

#include "model/TaskDate.h"

class Task final {
 public:
  enum class Priority {
    kHigh,
    kMedium,
    kLow,
    kNone,
  };

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

#endif  // TASKMANAGER_SRC_MODEL_TASK_H_
