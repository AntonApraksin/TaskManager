#ifndef TASKMANAGER_TESTS_TEST_UTILS_UTILS_H_
#define TASKMANAGER_TESTS_TEST_UTILS_UTILS_H_

#include <google/protobuf/util/time_util.h>

#include "model/SolidTask.h"
#include "test_utils/common.h"
#include "utils/TaskIdUtils.h"
#include "utils/TaskUtils.h"

inline SolidTask FindSolidTask(const SolidTasks& solid_tasks,
                               google::protobuf::int32 id) {
  return *std::find_if(solid_tasks.cbegin(), solid_tasks.cend(),
                       [id](const auto& i) { return i.task_id().id() == id; });
}

inline SolidTask TaskToSolidTask(const Task& task, google::protobuf::int32 id) {
  SolidTask solid_task;
  solid_task.set_allocated_task(new Task(task));
  solid_task.set_allocated_task_id(new TaskId(CreateTaskId(id)));
  return solid_task;
}

inline SolidTask TaskToSolidTask(const Task& task, google::protobuf::int32 id,
                                 google::protobuf::int32 parent_id) {
  auto solid_task = TaskToSolidTask(task, id);
  auto parent_task_id = CreateTaskId(parent_id);
  solid_task.set_allocated_parent_id(new TaskId(parent_task_id));
  return solid_task;
}

inline TaskDate_t parse_date(std::string date) {
  tm timeinfo;
  std::string pattern{"%d/%m/%y"};
  strptime(date.c_str(), pattern.c_str(), &timeinfo);
  return google::protobuf::util::TimeUtil::TimeTToTimestamp(
      std::mktime(&timeinfo));
}

#endif  // TASKMANAGER_TESTS_TEST_UTILS_UTILS_H_
