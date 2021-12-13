#include "IostreamStep.h"
#include "repl/view/steps/iostream/IostreamStrings.h"

IostreamShowSortedTasksStep::IostreamShowSortedTasksStep(
    const std::shared_ptr<IIoFacility> &io_facility, Tasks tasks)
    : IShowSortedTasksStep(tasks), IostreamStep(io_facility) {}

StepResult IostreamShowSortedTasksStep::Run() {
  for (const auto &i : tasks_) {
    io_facility_->Print(IostreamStrings::ShowTaskWithId(i.second, i.first));
  }
  return {{}, {}};
}