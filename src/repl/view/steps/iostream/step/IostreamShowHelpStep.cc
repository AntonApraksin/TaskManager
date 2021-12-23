#include "IostreamStep.h"
#include "repl/view/steps/Strings.h"

namespace task_manager {
IostreamShowHelpStep::IostreamShowHelpStep(
    const std::shared_ptr<IIoFacility>& io_facility)
    : IostreamStep(io_facility) {}

StepResult IostreamShowHelpStep::Run() {
  io_facility_->Print(Strings::kHelp);
  return {{}, {}};
}
}  // namespace task_manager