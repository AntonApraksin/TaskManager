#include "IostreamStep.h"
#include "repl/view/steps/iostream/IostreamStrings.h"

IostreamShowAllTasksStep::IostreamShowAllTasksStep(
    const std::shared_ptr<IIoFacility> &io_facility,
    TaskStorageRef task_storage)
    : IShowAllTasksStep(task_storage), IostreamStep(io_facility) {}

StepResult IostreamShowAllTasksStep::Run() {
  for (const auto &i : task_storage_.get().ShowStorage()) {
    io_facility_->Print(IostreamStrings::ShowTaskWithId(*i.second, i.first));
    io_facility_->Print(IostreamStrings::ShowNestedMap(i.second, 3));
  }
  return {{}, {}};
}