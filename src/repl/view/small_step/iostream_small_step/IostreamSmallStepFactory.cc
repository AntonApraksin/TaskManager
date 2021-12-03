#include "IostreamSmallStepFactory.h"

#define GENERATE_GETTER_FOR_POINTER(cls, name, args) \
  if (name == nullptr) {                             \
    name = std::make_shared<cls> args;               \
  }                                                  \
  return name;

IostreamSmallStepFactory::IostreamSmallStepFactory(
    const std::shared_ptr<IIOFacility>& printer,
    const std::shared_ptr<IValidator>& validator)
    : printer_(printer), validator_(validator) {}

std::shared_ptr<IostreamSmallStep> IostreamSmallStepFactory::GetREPLState(
    IostreamSmallStepEnum sse) {
  switch (sse) {
    case IostreamSmallStepEnum::kReadTitle:
      return GetIostreamReadTitleSmallStep();
    case IostreamSmallStepEnum::kReadDate:
      return GetIostreamReadDateSmallStep();
    case IostreamSmallStepEnum::kReadPriority:
      return GetIostreamReadPrioritySmallStep();
  }
}

std::shared_ptr<IostreamReadTitleSmallStep>
IostreamSmallStepFactory::GetIostreamReadTitleSmallStep() {
  GENERATE_GETTER_FOR_POINTER(IostreamReadTitleSmallStep, read_title_repl_sub_state_,
                              (printer_, validator_));
}

std::shared_ptr<IostreamReadDateSmallStep>
IostreamSmallStepFactory::GetIostreamReadDateSmallStep() {
  GENERATE_GETTER_FOR_POINTER(IostreamReadDateSmallStep, read_date_repl_sub_state_,
                              (printer_, validator_));
}

std::shared_ptr<IostreamReadPrioritySmallStep>
IostreamSmallStepFactory::GetIostreamReadPrioritySmallStep() {
  GENERATE_GETTER_FOR_POINTER(IostreamReadPrioritySmallStep,
                              read_priority_repl_sub_state_,
                              (printer_, validator_));
}