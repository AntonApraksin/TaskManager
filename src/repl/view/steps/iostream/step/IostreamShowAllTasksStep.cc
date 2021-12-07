#include "IostreamStep.h"
#include "repl/view/steps/iostream/IostreamGeneralFunctional.h"

IostreamShowAllTasksStep::IostreamShowAllTasksStep(
    const std::shared_ptr<IIoFacility> &io_facility,
    TaskStorageRef task_storage)
    : IShowAllTasksStep(task_storage), IostreamStep(io_facility) {}

StepResult IostreamShowAllTasksStep::Run() {
  for (const auto &i : task_storage_.get().ShowStorage()) {
    ShowTaskWithId(*io_facility_, *i.second, i.first);
    ShowNestedMap(*io_facility_, i.second, 3);
  }
  return {{}, {}};
}