#include "DefaultStep.h"
#include "repl/view/steps/Strings.h"

namespace task_manager {
DefaultReportMessageStep::DefaultReportMessageStep(
    const std::shared_ptr<IIoFacility>& io_facility)
    : DefaultStep(io_facility) {}

StepResult DefaultReportMessageStep::Run() {
  io_facility_->Print(message_);
  return {{}, {}};
}
}  // namespace task_manager