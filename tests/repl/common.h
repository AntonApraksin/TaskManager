#ifndef TASKMANAGER_TESTS_REPL_COMMON_H_
#define TASKMANAGER_TESTS_REPL_COMMON_H_

#include <google/protobuf/util/time_util.h>

#include "../common.h"
#include "model/task/Task.h"
#include "repl/validator/DateFormat.h"
#include "repl/view/steps/iostream/IostreamStrings.h"

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
    Date_t due_date =
        google::protobuf::util::TimeUtil::TimeTToTimestamp(std::time(nullptr));
    auto time = google::protobuf::util::TimeUtil::TimestampToTimeT(due_date);
    Task::Priority priority = static_cast<Task::Priority>(state_ % 3);
    Task::Progress progress = static_cast<Task::Progress>(state_ % 2);
    ++state_;
    std::stringstream ss_date;
    ss_date << std::put_time(std::localtime(&time), kDatePattern);
    return {ss.str(), ss_date.str(), IostreamStrings::to_string(priority),
            IostreamStrings::to_string(progress)};
  }

 private:
  int state_ = 0;
};

#endif  // TASKMANAGER_TESTS_REPL_COMMON_H_
