#include "interactor/state_machine/interactor_steps/ShowStep.h"

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/PromptStep.h"
#include "utils/TaskIdUtils.h"

namespace task_manager {
std::unique_ptr<Command> ShowStep::execute(Context ctx) {
  --stage_;
  if (stage_ == 1) {
    return HandleStage<1>(ctx);
  }
  if (stage_ == 0) {
    return HandleStage<0>(ctx);
  }
  std::terminate();
}

void ShowStep::ChangeStep(std::shared_ptr<Step> &active_step) {
  if (stage_ == 0) {
    active_step = std::make_shared<PromptStep>(validator_, io_facility_,
                                               small_step_factory_);
  }
}

template <>
std::unique_ptr<Command> ShowStep::HandleStage<1>(Context &) {
  if (arg_.empty()) {
    return std::make_unique<GetAllTasksCommand>();
  }
  std::string token;
  std::optional<int> to_show;
  for (; !arg_.empty();) {
    token = validator_->ConsumeOneTokenFrom(arg_);
    to_show = validator_->ParseInt(token);
    if (!to_show) {
      return ReportError(Strings::InvalidId(token));
    }
    task_ids_.push_back(CreateTaskId(*to_show));
  }
  if (task_ids_.size() >
      std::set<TaskId>(task_ids_.begin(), task_ids_.end()).size()) {
    return ReportError(Strings::kRepeatedId);
  }
  return std::make_unique<GetSpecifiedTasksCommand>(task_ids_);
}

template <>
std::unique_ptr<Command> ShowStep::HandleStage<0>(Context &ctx) {
  if (!ctx.solid_tasks &&
      ctx.status == ModelController::Status::kNotPresentId) {
    return ReportError(Strings::kNotPresentId);
  }
  if (ctx.solid_tasks) {
    io_facility_->Print(Strings::ShowSolidTasks(*ctx.solid_tasks));
  }
  return std::make_unique<VoidCommand>();
}

std::unique_ptr<Command> ShowStep::ReportError(std::string str) {
  stage_ = 0;
  io_facility_->Print(str);
  return std::make_unique<VoidCommand>();
}
}  // namespace task_manager
