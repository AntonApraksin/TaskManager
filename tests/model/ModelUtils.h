#ifndef TASKMANAGER_TESTS_MODEL_MODELUTILS_H_
#define TASKMANAGER_TESTS_MODEL_MODELUTILS_H_

#include <sstream>

#include "model/id/ITaskIdProducer.h"
#include "model/id/TaskId.h"
#include "model/task/Task.h"

inline bool operator!=(const TaskId& lhs, const TaskId& rhs) {
  return !(lhs == rhs);
}

inline bool operator==(const Task& lhs, const Task& rhs) {
  return lhs.GetTitle() == rhs.GetTitle() &&
         lhs.GetPriority() == rhs.GetPriority() &&
         lhs.GetDueDate() == rhs.GetDueDate();
}

class MockTaskIdProducer : public ITaskIdProducer {
 public:
  TaskId GetNextId() override { return TaskId::Create(current_++); }

 private:
  int current_ = 0;
};

class TaskFactory final {
 public:
  Task GetNextTask() {
    std::stringstream ss;
    ss << "Sample task #" << state_;
    Date_t time =
        std::chrono::system_clock::now() + std::chrono::seconds(state_);
    Task::Priority priority = static_cast<Task::Priority>(state_ % 4);
    Task::State state = static_cast<Task::State>(state_ % 3);
    ++state_;
    return *Task::Create(ss.str(), priority, time, state);
  }

 private:
  int state_ = 0;
};

#endif  // TASKMANAGER_TESTS_MODEL_MODELUTILS_H_
