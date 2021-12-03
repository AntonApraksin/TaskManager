#include "DefaultStateFactory.h"

#define GENERATE_GETTER_FOR_POINTER(cls, name, args) \
  if (name == nullptr) {                             \
    name = std::make_shared<cls> args;               \
  }                                                  \
  return name;

DefaultStateFactory::DefaultStateFactory(
    const std::shared_ptr<IIOFacility>& printer,
    const std::shared_ptr<IValidator>& validator)
    : printer_(printer), validator_(validator) {}

std::shared_ptr<IREPLSubState> DefaultStateFactory::GetREPLState(
    SubStateEnum sse) {
  switch (sse) {
    case SubStateEnum::kReadTitle:
      return GetReadTitleREPLSubState();
    case SubStateEnum::kReadDate:
      return GetReadDateREPLSubState();
    case SubStateEnum::kReadPriority:
      return GetReadPriorityREPLSubState();
  }
}

std::shared_ptr<ReadTitleREPLSubState>
DefaultStateFactory::GetReadTitleREPLSubState() {
  GENERATE_GETTER_FOR_POINTER(ReadTitleREPLSubState, read_title_repl_sub_state_,
                              (printer_, validator_));
}

std::shared_ptr<ReadDateREPLSubState>
DefaultStateFactory::GetReadDateREPLSubState() {
  GENERATE_GETTER_FOR_POINTER(ReadDateREPLSubState, read_date_repl_sub_state_,
                              (printer_, validator_));
}

std::shared_ptr<ReadPriorityREPLSubState>
DefaultStateFactory::GetReadPriorityREPLSubState() {
  GENERATE_GETTER_FOR_POINTER(ReadPriorityREPLSubState,
                              read_priority_repl_sub_state_,
                              (printer_, validator_));
}