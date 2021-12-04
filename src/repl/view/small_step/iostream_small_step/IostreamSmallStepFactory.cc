#include "IostreamSmallStepFactory.h"

#define GENERATE_GETTER_FOR_POINTER(cls, name, args) \
  if (name == nullptr) {                             \
    name = std::make_shared<cls> args;               \
  }                                                  \
  return name;

IostreamSmallStepFactory::IostreamSmallStepFactory(
    const std::shared_ptr<IValidator>& validator)
    : validator_(validator) {}

std::shared_ptr<IostreamSmallStep> IostreamSmallStepFactory::GetREPLState(
    IostreamSmallStepEnum sse) {
  switch (sse) {
    case IostreamSmallStepEnum::kReadTitle:
      return GetIostreamReadTitleSmallStep();
    case IostreamSmallStepEnum::kReadDate:
      return GetIostreamReadDateSmallStep();
    case IostreamSmallStepEnum::kReadPriority:
      return GetIostreamReadPrioritySmallStep();
    case IostreamSmallStepEnum::kReadState:
      return GetIostreamReadStateSmallStep();
  }
}

std::shared_ptr<IostreamReadTitleSmallStep>
IostreamSmallStepFactory::GetIostreamReadTitleSmallStep() {
  GENERATE_GETTER_FOR_POINTER(IostreamReadTitleSmallStep,
                              read_title_repl_sub_state_, (validator_));
}

std::shared_ptr<IostreamReadStateSmallStep>
IostreamSmallStepFactory::GetIostreamReadStateSmallStep() {
  GENERATE_GETTER_FOR_POINTER(IostreamReadStateSmallStep,
                              iostream_read_state_small_step_, (validator_));
}

std::shared_ptr<IostreamReadDateSmallStep>
IostreamSmallStepFactory::GetIostreamReadDateSmallStep() {
  GENERATE_GETTER_FOR_POINTER(IostreamReadDateSmallStep,
                              read_date_repl_sub_state_, (validator_));
}

std::shared_ptr<IostreamReadPrioritySmallStep>
IostreamSmallStepFactory::GetIostreamReadPrioritySmallStep() {
  GENERATE_GETTER_FOR_POINTER(IostreamReadPrioritySmallStep,
                              read_priority_repl_sub_state_, (validator_));
}