#ifndef TASKMANAGER_TESTS_TEST_UTILS_TASKFACTORY_H_
#define TASKMANAGER_TESTS_TEST_UTILS_TASKFACTORY_H_

#include <google/protobuf/util/time_util.h>

#include <sstream>

#include "Task.pb.h"
#include "TaskId.pb.h"
#include "test_utils/common.h"
#include "utils/TaskUtils.h"

class TaskFactory final {
 public:
  Task GetNextTask() {
    std::stringstream ss;
    ss << "Sample task #" << state_;
    TaskDate_t due_date =
        google::protobuf::util::TimeUtil::TimeTToTimestamp(std::time(nullptr));
    Task::Priority priority = static_cast<Task::Priority>(state_ % 3);
    Task::Progress progress = Task::kUncompleted;
    ++state_;
    return *CreateTask(ss.str(), due_date, priority, progress);
  }

 private:
  int state_ = 0;
};

#endif  // TASKMANAGER_TESTS_TEST_UTILS_TASKFACTORY_H_
