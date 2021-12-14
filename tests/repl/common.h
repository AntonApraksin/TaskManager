#ifndef TASKMANAGER_TESTS_REPL_COMMON_H_
#define TASKMANAGER_TESTS_REPL_COMMON_H_

#include "model/task/Task.h"
#include "repl/validator/DateFormat.h"
#include "repl/view/steps/iostream/IostreamStrings.h"

inline bool operator==(const Task& lhs,
                       const Task& rhs) {  // TODO: find it a better place
  return lhs.GetTitle() == rhs.GetTitle() &&
         lhs.GetPriority() == rhs.GetPriority() &&
         lhs.GetDueDate() == rhs.GetDueDate();
}

struct TaskStringedData {
  std::string title;
  std::string date;
  std::string priority;
  std::string state;
};

class TaskStringedDataProducer final {
 public:
  TaskStringedData GetData() {
    std::stringstream ss;
    ss << "Sample task #" << state_;
    Date_t chrono_time =
        std::chrono::system_clock::now() + std::chrono::seconds(state_);
    auto time = std::chrono::system_clock::to_time_t(chrono_time);
    Task::Priority priority = static_cast<Task::Priority>(state_ % 3);
    Task::State state = static_cast<Task::State>(state_ % 2);
    ++state_;
    std::stringstream ss_date;
    ss_date << std::put_time(std::localtime(&time), kDatePattern);
    return {ss.str(), ss_date.str(), IostreamStrings::to_string(priority),
            IostreamStrings::to_string(state)};
  }

 private:
  int state_ = 0;
};

#endif  // TASKMANAGER_TESTS_REPL_COMMON_H_
