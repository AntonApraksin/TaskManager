#include "interactor/state_machine/interactor_steps/DeleteStep.h"

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/PromptStep.h"
#include "utils/TaskIdUtils.h"

namespace task_manager {
std::unique_ptr<Command> DeleteStep::execute(Context ctx) {
  --stage_;
  if (stage_ == 1) {
    return HandleStage<1>(ctx);
  }
  if (stage_ == 0) {
    return HandleStage<0>(ctx);
  }
  std::terminate();
}

void DeleteStep::ChangeStep(std::shared_ptr<Step> &active_step) {
  if (stage_ == 0) {
    active_step = std::make_shared<PromptStep>(validator_, io_facility_,
                                               small_step_factory_);
  }
}

template <>
std::unique_ptr<Command> DeleteStep::HandleStage<1>(Context &) {
  if (arg_.empty()) {
    return ReportError(Strings::kRequiredId);
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
std::unique_ptr<Command> DeleteStep::HandleStage<0>(Context &ctx) {
  if (!ctx.solid_tasks &&
      ctx.status == ModelController::Status::kNotPresentId) {
    return ReportError(Strings::kNotPresentId);
  }

  auto has_parent_child_relationship =
      HasParentChildRelationship(*ctx.solid_tasks, task_ids_);
  if (has_parent_child_relationship) {
    return ReportError(Strings::IdIsSubIdOf(
        std::to_string(has_parent_child_relationship->first.id()),
        std::to_string(has_parent_child_relationship->second.id())));
  }
  io_facility_->Print(Strings::YouAreGoingTo("delete"));

  io_facility_->Print(Strings::ShowSolidTasks(*ctx.solid_tasks));

  io_facility_->Print(Strings::ProceedTo("delete"));
  std::string input = io_facility_->GetLine();
  auto confirm = validator_->ParseConfirmation(input);
  if (!confirm) {
    io_facility_->Print(Strings::kOkayITreatItAsNo);
    return std::make_unique<VoidCommand>();
  }
  if (*confirm == ConfirmationResult::kNo) {
    return std::make_unique<VoidCommand>();
  }
  return std::make_unique<DeleteTasksCommand>(std::move(task_ids_));
}

std::unique_ptr<Command> DeleteStep::ReportError(std::string str) {
  stage_ = 0;
  io_facility_->Print(str);
  return std::make_unique<VoidCommand>();
}
}  // namespace task_manager