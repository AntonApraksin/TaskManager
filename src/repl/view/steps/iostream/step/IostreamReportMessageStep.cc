#include "IostreamStep.h"
#include "repl/view/steps/Strings.h"

namespace task_manager {
IostreamReportMessageStep::IostreamReportMessageStep(
    const std::shared_ptr<IIoFacility>& io_facility)
    : IostreamStep(io_facility) {}

StepResult IostreamReportMessageStep::Run() {
  io_facility_->Print(message_);
  return {{}, {}};
}
}  // namespace task_manager