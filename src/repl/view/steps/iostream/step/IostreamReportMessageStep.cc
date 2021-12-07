#include <sstream>

#include "repl/view/steps/iostream/step/IostreamStep.h"

IostreamReportMessageStep::IostreamReportMessageStep(
    const std::shared_ptr<IIoFacility>& io_facility)
    : IostreamStep(io_facility) {}

StepResult IostreamReportMessageStep::Run() {
  switch (main_error_) {
    case MessageEnum::kUnknownCommand:
      io_facility_->Print(
          "Unknown command. Type 'help' to display all available commands.\n");
      break;

    case MessageEnum::kInvalidId:
      io_facility_->Print("Id should be numeric value.\n");
      break;

    case MessageEnum::kNotPresentId: {
      std::stringstream ss;
      ss << "Id " << task_id_->GetId() << " not present in store.\n";
      io_facility_->Print(ss.str());
      break;
    }

    case MessageEnum::kMultipleId:
      io_facility_->Print("Multiple Id does not supported for this command.\n");
      break;

    case MessageEnum::kRequiredId:
      io_facility_->Print("Id is required for this command.\n");
      break;

    case MessageEnum::kShowId: {
      std::stringstream ss;
      ss << "Id " << task_id_->GetId() << ".\n";
      io_facility_->Print(ss.str());
      break;
    }
  }
  return {{}, {}};
}