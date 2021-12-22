#include "TaskContext.h"

void TaskContext::PushState(const std::shared_ptr<ISmallStep>& state) {
  states_.push_back(state);
}

void TaskContext::PopState() { states_.pop_front(); }

TaskBuilder& TaskContext::GetTaskBuilder() { return task_builder_; }

void TaskContext::Run() {
  for (; !states_.empty();) {
    auto to_run_on = states_.front();
    to_run_on->Execute(*this);
  }
}