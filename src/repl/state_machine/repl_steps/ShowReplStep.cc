#include "repl/state_machine/repl_steps/ShowReplStep.h"

#include "repl/io_facility/Strings.h"
#include "repl/state_machine/repl_steps/PromptReplStep.h"
#include "utils/TaskIdUtils.h"

namespace task_manager {
std::unique_ptr<Command> ShowReplStep::execute(Context ctx) {
  --stage_;
  if (stage_ == 1) {
    return HandleStage<1>(ctx);
  }
  if (stage_ == 0) {
    return HandleStage<0>(ctx);
  }
  std::terminate();
}

void ShowReplStep::ChangeStep(std::shared_ptr<ReplStep> &active_step) {
  if (stage_ == 0) {
    active_step = std::make_shared<PromptReplStep>(validator_, io_facility_,
                                                   small_step_factory_);
  }
}

template <>
std::unique_ptr<Command> ShowReplStep::HandleStage<1>(Context &) {
  if (arg_.empty()) {
    return std::make_unique<GetAllTasksCommand>();
  }
  std::string token;
  std::optional<int> to_complete;
  for (; !arg_.empty();) {
    token = validator_->ConsumeOneTokenFrom(arg_);
    to_complete = validator_->ParseInt(token);
    if (!to_complete) {
      return ReportError(Strings::InvalidId(token));
    }
    task_ids_.push_back(CreateTaskId(*to_complete));
  }
  if (task_ids_.size() >
      std::set<TaskId>(task_ids_.begin(), task_ids_.end()).size()) {
    return ReportError(Strings::kRepeatedId);
  }
  return std::make_unique<GetSpecifiedTasksCommand>(task_ids_);
}

template <>
std::unique_ptr<Command> ShowReplStep::HandleStage<0>(Context &ctx) {
  if (!ctx.solid_tasks &&
      ctx.status == ModelController::Status::kNotPresentId) {
    return ReportError(Strings::kNotPresentId);
  }
  if (ctx.solid_tasks) {
    io_facility_->Print(Strings::ShowSolidTasks(*ctx.solid_tasks));
  }
  return std::make_unique<VoidCommand>();
}

std::unique_ptr<Command> ShowReplStep::ReportError(std::string str) {
  stage_ = 0;
  io_facility_->Print(str);
  return std::make_unique<VoidCommand>();
}
}  // namespace task_manager
