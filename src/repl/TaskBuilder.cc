#include "TaskBuilder.h"

void TaskBuilder::SetTitle(const std::string& title) { title_ = title; }
void TaskBuilder::SetPriority(Task::Priority priority) { priority_ = priority; }
void TaskBuilder::SetDate(Date_t date) { date_ = date; }
void TaskBuilder::SetState(Task::State state) { state_ = state; }

Task TaskBuilder::GetTask() {
  return *Task::Create(title_, priority_, date_, state_);
}
