#include "repl/view/steps/Strings.h"
#include "repl/view/steps/iostream/step/IostreamStep.h"

IostreamReportMessageStep::IostreamReportMessageStep(
    const std::shared_ptr<IIoFacility>& io_facility)
    : IostreamStep(io_facility) {}

StepResult IostreamReportMessageStep::Run() {
  io_facility_->Print(message_);
  return {{}, {}};
}