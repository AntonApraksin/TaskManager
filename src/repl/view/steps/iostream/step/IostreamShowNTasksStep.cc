#include "IostreamStep.h"
#include "repl/view/steps/iostream/IostreamStrings.h"

IostreamShowNTasksStep::IostreamShowNTasksStep(
    const std::shared_ptr<IIoFacility> &io_facility, TaskWrappers task_wrappers)
    : IShowNTasksStep(task_wrappers), IostreamStep(io_facility) {}

StepResult IostreamShowNTasksStep::Run() {
  for (const auto &i : task_wrappers_) {
    io_facility_->Print(IostreamStrings::ShowTask(*i.get()));
    io_facility_->Print(IostreamStrings::ShowNestedMap(i, 2));
  }
  return {{}, {}};
}