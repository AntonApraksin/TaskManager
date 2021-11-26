#include "Context.h"

Context::Context(std::unique_ptr<IStateFactory> state_factory)
    : task_id_(TaskId::Create(0)),
      state_factory_(std::move(state_factory)),
      state_(state_factory_->GetState(StateEnum::kDefault)) {}

void Context::Run() {
  for (; state_ != nullptr;) {
    state_ = state_->Execute(*this);
  }
}

IStateFactory& Context::GetStateFactory() { return *state_factory_; }

TaskBuilder& Context::GetTaskBuilder() { return task_builder_; }

TaskId& Context::GetTaskId() { return task_id_; }

void Context::RunInteractor() {
  for (; sub_state_ != nullptr;) {
    sub_state_ = sub_state_->Execute(*this);
  }
}

void Context::Commit() { commit_state_->Execute(*this); }

void Context::SetSubState(const std::shared_ptr<IREPLSubState>& sub_state) {
  sub_state_ = sub_state;
}
void Context::SetCommitState(
    const std::shared_ptr<ICommitState>& commit_state) {
  commit_state_ = commit_state;
}
