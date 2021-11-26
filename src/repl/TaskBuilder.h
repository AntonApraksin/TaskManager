#ifndef TASKMANAGER_SRC_REPL_TASKBUILDER_H_
#define TASKMANAGER_SRC_REPL_TASKBUILDER_H_

#include "model/task/Task.h"

class TaskBuilder {
 public:
  void SetTitle(const std::string& title);
  void SetPriority(Task::Priority priority);
  void SetDate(Date_t date);
  void SetState(Task::State state);

  Task GetTask();

 private:
  std::string title_;
  Task::Priority priority_;
  Date_t date_;
  Task::State state_ = Task::State::kUncompleted;
};

#endif  // TASKMANAGER_SRC_REPL_TASKBUILDER_H_
