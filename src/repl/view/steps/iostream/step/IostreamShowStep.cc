#include "IostreamStep.h"
#include "repl/view/steps/iostream/IostreamStrings.h"

IostreamShowStep::IostreamShowStep(
    const std::shared_ptr<IIoFacility> &io_facility)
    : IostreamStep(io_facility) {}

StepResult IostreamShowStep::Run() {
  io_facility_->Print(IostreamStrings::ShowSolidTasks(solid_tasks_));
  return {{}, {}};
}