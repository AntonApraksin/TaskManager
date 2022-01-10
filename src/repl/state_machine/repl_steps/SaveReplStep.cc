#include "repl/state_machine/repl_steps/SaveReplStep.h"

#include <fstream>

#include "repl/io_facility/Strings.h"
#include "repl/state_machine/repl_steps/PromptReplStep.h"

namespace task_manager {
std::unique_ptr<Command> SaveReplStep::execute(Context ctx) {
  --stage_;
  if (stage_ == 1) {
    return HandleStage<1>(ctx);
  }
  if (stage_ == 0) {
    return HandleStage<0>(ctx);
  }
  std::terminate();
}

void SaveReplStep::ChangeStep(std::shared_ptr<ReplStep> &active_step) {
  if (stage_ == 0) {
    active_step = std::make_shared<PromptReplStep>(validator_, io_facility_,
                                                   small_step_factory_);
  }
}

template <>
std::unique_ptr<Command> SaveReplStep::HandleStage<1>(Context &) {
  filename_ = validator_->ConsumeOneTokenFrom(arg_);
  if (!arg_.empty()) {
    ReportError(Strings::kMultipleArgumentDoesNotSupported);
  }
  auto file{std::make_unique<std::ofstream>(filename_)};
  if (!file->is_open()) {
    return ReportError(Strings::ErrorDuringOpeningFile(filename_));
  }
  return std::make_unique<SaveTasksCommand>(std::move(file));
}

template <>
std::unique_ptr<Command> SaveReplStep::HandleStage<0>(Context &ctx) {
  if (ctx.status == ModelController::Status::kSaveFailure) {
    return ReportError(Strings::FailureDuringSaving(filename_));
  }
  return std::make_unique<VoidCommand>();
}

std::unique_ptr<Command> SaveReplStep::ReportError(std::string str) {
  stage_ = 0;
  io_facility_->Print(str);
  return std::make_unique<VoidCommand>();
}
}  // namespace task_manager