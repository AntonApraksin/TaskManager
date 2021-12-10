#ifndef TASKMANAGER_SRC_MODEL_TASK_TASK_H_
#define TASKMANAGER_SRC_MODEL_TASK_TASK_H_
#pragma once

#include <optional>
#include <string>

#include "TaskDate.h"

class Task final {
 public:
  enum class Priority {
    kHigh,
    kMedium,
    kLow,
  };

  enum class State {
    kUncompleted,
    kCompleted,
  };

 public:
  static std::optional<Task> Create(std::string, Priority, Date_t,
                                    State = State::kUncompleted);

 public:
  std::string GetTitle() const;
  Priority GetPriority() const;
  Date_t GetDueDate() const;
  State GetState() const;

 private:
  Task(std::string, Priority, Date_t, State);

 private:
  std::string title_;
  Priority priority_;
  Date_t due_date_;
  State state_;
};

#endif  // TASKMANAGER_SRC_MODEL_TASK_TASK_H_
