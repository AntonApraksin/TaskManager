#include "IostreamSmallStepFactory.h"

#define GENERATE_GETTER_FOR_POINTER(cls, name, args) \
  if (name == nullptr) {                             \
    name = std::make_shared<cls> args;               \
  }                                                  \
  return name;

namespace task_manager {
IostreamSmallStepFactory::IostreamSmallStepFactory(
    const std::shared_ptr<IIoFacility>& io_facility,
    const std::shared_ptr<IValidator>& validator)
    : validator_(validator), io_facility_(io_facility) {}

std::shared_ptr<IostreamReadTitleSmallStep>
IostreamSmallStepFactory::GetIostreamReadTitleSmallStep(){
    GENERATE_GETTER_FOR_POINTER(IostreamReadTitleSmallStep,
                                iostream_read_title_small_step_,
                                (io_facility_, validator_))}

std::shared_ptr<IostreamReadStateSmallStep> IostreamSmallStepFactory::
    GetIostreamReadStateSmallStep(){
        GENERATE_GETTER_FOR_POINTER(IostreamReadStateSmallStep,
                                    iostream_read_state_small_step_,
                                    (io_facility_, validator_))}

std::shared_ptr<IostreamReadDateSmallStep> IostreamSmallStepFactory::
    GetIostreamReadDateSmallStep(){
        GENERATE_GETTER_FOR_POINTER(IostreamReadDateSmallStep,
                                    iostream_read_date_small_step_,
                                    (io_facility_, validator_))}

std::shared_ptr<IostreamReadPrioritySmallStep> IostreamSmallStepFactory::
    GetIostreamReadPrioritySmallStep(){
        GENERATE_GETTER_FOR_POINTER(IostreamReadPrioritySmallStep,
                                    iostream_read_priority_small_step_,
                                    (io_facility_, validator_))}

std::shared_ptr<ISmallStep> IostreamSmallStepFactory::GetReadTitleSmallStep() {
  return GetIostreamReadTitleSmallStep();
}

std::shared_ptr<ISmallStep> IostreamSmallStepFactory::GetReadDateSmallStep() {
  return GetIostreamReadDateSmallStep();
}

std::shared_ptr<ISmallStep>
IostreamSmallStepFactory::GetReadPrioritySmallStep() {
  return GetIostreamReadPrioritySmallStep();
}

std::shared_ptr<ISmallStep> IostreamSmallStepFactory::GetReadStateSmallStep() {
  return GetIostreamReadStateSmallStep();
}
}  // namespace task_manager