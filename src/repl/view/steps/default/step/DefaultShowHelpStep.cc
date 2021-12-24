#include "DefaultStep.h"
#include "repl/view/steps/Strings.h"

namespace task_manager {
DefaultShowHelpStep::DefaultShowHelpStep(
    const std::shared_ptr<IIoFacility>& io_facility)
    : DefaultStep(io_facility) {}

StepResult DefaultShowHelpStep::Run() {
  io_facility_->Print(Strings::kHelp);
  return {{}, {}};
}
}  // namespace task_manager