#include <iostream>

#include "IostreamStep.h"
#include "repl/view/steps/iostream/IostreamGeneralFunctional.h"

IostreamShowSortedTasksStep::IostreamShowSortedTasksStep(
    const std::shared_ptr<IIoFacility> &io_facility, Tasks tasks)
    : IShowSortedTasksStep(tasks), IostreamStep(io_facility) {}

StepResult IostreamShowSortedTasksStep::Run() {
  for (const auto &i : tasks_) {
    ShowTaskWithId(i.second, i.first);
  }
  return {{}, {}};
}