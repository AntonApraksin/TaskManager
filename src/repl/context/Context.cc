#include "Context.h"

Context::Context(const std::shared_ptr<IStateFactory>& state)
: state_(state) {}

void Context::Run() {
  for (; state_ != nullptr;) {
    state_ = state_->Execute(*this);
  }
}