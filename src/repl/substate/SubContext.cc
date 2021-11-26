#include "SubContext.h"

void SubContext::PushState(const std::shared_ptr<ISubState>& state) {
  states_.push_back(state);
}

void SubContext::PopState() { states_.pop_front(); }

void SubContext::ClearStates() { states_.clear(); }

TaskBuilder& SubContext::GetTaskBuilder()
{
  return task_builder_;
}

TaskId& SubContext::GetTaskId() {
  return task_id_;
}

void SubContext::Run()
{
  for(; !states_.empty();){
    auto to_run_on = states_.front();
    to_run_on->Execute(*this);
  }
}