#include "interactor/state_machine/interactor_steps/ShowStep.h"

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/FinalizeStep.h"
#include "utils/TaskIdUtils.h"

namespace task_manager {
std::unique_ptr<Command> ShowStep::execute(StepParameter &) {
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

void ShowStep::ChangeStep(std::shared_ptr<Step> &active_step) {
  active_step = std::make_shared<FinalizeStep>(validator_, io_facility_,
                                               small_step_factory_);
}

std::unique_ptr<Command> ShowStep::ReportError(std::string str) {
  io_facility_->Print(str);
  return std::make_unique<VoidCommand>();
}
}  // namespace task_manager
