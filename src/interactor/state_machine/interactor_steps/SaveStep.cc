#include "interactor/state_machine/interactor_steps/SaveStep.h"

#include <fstream>

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/PromptStep.h"

namespace task_manager {
std::unique_ptr<Command> SaveStep::execute(Context ctx) {
  --stage_;
  if (stage_ == 1) {
    return HandleStage<1>(ctx);
  }
  if (stage_ == 0) {
    return HandleStage<0>(ctx);
  }
  std::terminate();
}

void SaveStep::ChangeStep(std::shared_ptr<Step> &active_step) {
  if (stage_ == 0) {
    active_step = std::make_shared<PromptStep>(validator_, io_facility_,
                                               small_step_factory_);
  }
}

template <>
std::unique_ptr<Command> SaveStep::HandleStage<1>(Context &) {
  if (arg_.empty()) {
    return ReportError(Strings::kMultipleArgumentDoesNotSupported);
  }
  filename_ = validator_->ConsumeOneTokenFrom(arg_);
  if (!arg_.empty()) {
    return ReportError(Strings::kMultipleArgumentDoesNotSupported);
  }
  return std::make_unique<SaveTasksToFileCommand>(filename_);
}

template <>
std::unique_ptr<Command> SaveStep::HandleStage<0>(Context &ctx) {
  if (ctx.status == ModelController::Status::kSaveFailure) {
    return ReportError(Strings::FailureDuringSaving(filename_));
  }
  return std::make_unique<VoidCommand>();
}

std::unique_ptr<Command> SaveStep::ReportError(std::string str) {
  stage_ = 0;
  io_facility_->Print(str);
  return std::make_unique<VoidCommand>();
}
}  // namespace task_manager
