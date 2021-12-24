#include "DefaultStep.h"
#include "repl/view/steps/Strings.h"

namespace task_manager {
DefaultShowStep::DefaultShowStep(
    const std::shared_ptr<IIoFacility> &io_facility)
    : DefaultStep(io_facility) {}

StepResult DefaultShowStep::Run() {
  io_facility_->Print(Strings::ShowSolidTasks(solid_tasks_));
  return {{}, {}};
}
}  // namespace task_manager