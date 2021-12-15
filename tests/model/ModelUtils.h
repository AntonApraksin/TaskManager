#ifndef TASKMANAGER_TESTS_MODEL_MODELUTILS_H_
#define TASKMANAGER_TESTS_MODEL_MODELUTILS_H_

#include <google/protobuf/util/time_util.h>

#include <sstream>

#include "../common.h"
#include "model/id/ITaskIdProducer.h"
#include "model/id/TaskId.h"
#include "model/task/Task.h"

inline Date_t parse_date(std::string date) {
  tm timeinfo;
  std::string pattern{"%d/%m/%y"};
  strptime(date.c_str(), pattern.c_str(), &timeinfo);
  return google::protobuf::util::TimeUtil::TimeTToTimestamp(
      std::mktime(&timeinfo));
}

class MockTaskIdProducer : public ITaskIdProducer {
 public:
  TaskId GetNextId() override { return CreateTaskId(current_++); }

 private:
  int current_ = 0;
};

class TaskFactory final {
 public:
  Task GetNextTask() {
    std::stringstream ss;
    ss << "Sample task #" << state_;
    Date_t due_date =
        google::protobuf::util::TimeUtil::TimeTToTimestamp(std::time(nullptr));
    Task::Priority priority = static_cast<Task::Priority>(state_ % 4);
    Task::Progress progress = static_cast<Task::Progress>(state_ % 3);
    ++state_;
    return *CreateTask(ss.str(), due_date, priority, progress);
  }

 private:
  int state_ = 0;
};

#endif  // TASKMANAGER_TESTS_MODEL_MODELUTILS_H_
