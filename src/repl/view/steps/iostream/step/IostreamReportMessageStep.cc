#include "repl/view/steps/iostream/IostreamStrings.h"
#include "repl/view/steps/iostream/step/IostreamStep.h"

IostreamReportMessageStep::IostreamReportMessageStep(
    const std::shared_ptr<IIoFacility>& io_facility)
    : IostreamStep(io_facility) {}

StepResult IostreamReportMessageStep::Run() {
  switch (main_error_) {
    case MessageEnum::kUnknownCommand:
      io_facility_->Print(IostreamStrings::kUnknownCommand);
      break;

    case MessageEnum::kInvalidId:
      if (arg_) {
        io_facility_->Print(IostreamStrings::InvalidId(*arg_));
      } else {
        io_facility_->Print(IostreamStrings::kInvalidId);
      }
      break;

    case MessageEnum::kNotPresentId:
      io_facility_->Print(IostreamStrings::NotPresentId(*arg_));
      break;

    case MessageEnum::kMultipleId:
      io_facility_->Print(IostreamStrings::kMultipleId);
      break;

    case MessageEnum::kRequiredId:
      io_facility_->Print(IostreamStrings::kRequiredId);
      break;

    case MessageEnum::kShowId:
      io_facility_->Print(IostreamStrings::ShowId(*arg_));
      break;

    case MessageEnum::kRepeatedId:
      io_facility_->Print(IostreamStrings::kRepeatedId);
      break;
  }
  return {{}, {}};
}