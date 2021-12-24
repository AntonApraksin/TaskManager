#include "DefaultSmallStepFactory.h"

#define GENERATE_GETTER_FOR_POINTER(cls, name, args) \
  if (name == nullptr) {                             \
    name = std::make_shared<cls> args;               \
  }                                                  \
  return name;

namespace task_manager {
DefaultSmallStepFactory::DefaultSmallStepFactory(
    const std::shared_ptr<IIoFacility>& io_facility,
    const std::shared_ptr<IValidator>& validator)
    : validator_(validator), io_facility_(io_facility) {}

std::shared_ptr<DefaultReadTitleSmallStep>
DefaultSmallStepFactory::GetDefaultReadTitleSmallStep(){
    GENERATE_GETTER_FOR_POINTER(DefaultReadTitleSmallStep,
                                default_read_title_small_step_,
                                (io_facility_, validator_))}

std::shared_ptr<DefaultReadStateSmallStep> DefaultSmallStepFactory::
    GetDefaultReadStateSmallStep(){
        GENERATE_GETTER_FOR_POINTER(DefaultReadStateSmallStep,
                                    default_read_state_small_step_,
                                    (io_facility_, validator_))}

std::shared_ptr<DefaultReadDateSmallStep> DefaultSmallStepFactory::
    GetDefaultReadDateSmallStep(){
        GENERATE_GETTER_FOR_POINTER(DefaultReadDateSmallStep,
                                    default_read_date_small_step_,
                                    (io_facility_, validator_))}

std::shared_ptr<DefaultReadPrioritySmallStep> DefaultSmallStepFactory::
    GetDefaultReadPrioritySmallStep(){
        GENERATE_GETTER_FOR_POINTER(DefaultReadPrioritySmallStep,
                                    default_read_priority_small_step_,
                                    (io_facility_, validator_))}

std::shared_ptr<ISmallStep> DefaultSmallStepFactory::GetReadTitleSmallStep() {
  return GetDefaultReadTitleSmallStep();
}

std::shared_ptr<ISmallStep> DefaultSmallStepFactory::GetReadDateSmallStep() {
  return GetDefaultReadDateSmallStep();
}

std::shared_ptr<ISmallStep>
DefaultSmallStepFactory::GetReadPrioritySmallStep() {
  return GetDefaultReadPrioritySmallStep();
}

std::shared_ptr<ISmallStep> DefaultSmallStepFactory::GetReadStateSmallStep() {
  return GetDefaultReadStateSmallStep();
}
}  // namespace task_manager