#include "Context.h"

Context::Context(std::unique_ptr<IStateFactory> state_factory)
    : state_factory_(std::move(state_factory)),
      state_(state_factory_->GetState(StateEnum::kDefault)) {}

void Context::Run() {
  for (; state_ != nullptr;) {
    state_ = state_->Execute(*this);
  }
}

IStateFactory& Context::GetStateFactory() { return *state_factory_; }
