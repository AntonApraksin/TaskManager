#include "repl/view/steps/Strings.h"
#include "repl/view/steps/iostream/step/IostreamStep.h"

IostreamShowHelpStep::IostreamShowHelpStep(
    const std::shared_ptr<IIoFacility>& io_facility)
    : IostreamStep(io_facility) {}

StepResult IostreamShowHelpStep::Run() {
  io_facility_->Print(Strings::kHelp);
  return {{}, {}};
}