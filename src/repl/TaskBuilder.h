#ifndef TASKMANAGER_SRC_REPL_TASKBUILDER_H_
#define TASKMANAGER_SRC_REPL_TASKBUILDER_H_

#include "model/task/Task.h"

struct TaskBuilder {
  Task GetTask();

  std::optional<std::string> title_;
  std::optional<Date_t> date_;
  std::optional<Task::Priority> priority_;
  std::optional<Task::Progress> progress_;
};

#endif  // TASKMANAGER_SRC_REPL_TASKBUILDER_H_
