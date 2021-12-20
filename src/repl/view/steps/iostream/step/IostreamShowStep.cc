#include "IostreamStep.h"
#include "repl/view/steps/Strings.h"

IostreamShowStep::IostreamShowStep(
    const std::shared_ptr<IIoFacility> &io_facility)
    : IostreamStep(io_facility) {}

StepResult IostreamShowStep::Run() {
  io_facility_->Print(Strings::ShowSolidTasks(solid_tasks_));
  return {{}, {}};
}