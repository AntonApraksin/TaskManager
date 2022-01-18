#include "AddLabelStep.h"

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/PromptStep.h"
#include "utils/TaskIdUtils.h"

namespace task_manager {

std::unique_ptr<Command> AddLabelStep::execute(Context ctx) {
  --stage_;
  if (stage_ == 1) {
    return HandleStage<1>(ctx);
  }
  if (stage_ == 0) {
    return HandleStage<0>(ctx);
  }
  std::terminate();
}

template <>
std::unique_ptr<Command> AddLabelStep::HandleStage<1>(Context &) {
  if (arg_.empty()) {
    return ReportError(Strings::kRequiredId);
  }

  auto token = validator_->ConsumeOneTokenFrom(arg_);
  auto to_edit = validator_->ParseInt(token);
  if (!to_edit) {
    return ReportError(Strings::InvalidId(token));
  }
  task_id_.set_id(*to_edit);

  if (arg_.empty()) {
    return ReportError(Strings::kRequiredLabel);
  }

  token = validator_->ConsumeOneTokenFrom(arg_);
  if (!arg_.empty()) {
    return ReportError(Strings::kMultipleArgumentDoesNotSupported);
  }

  label_.set_name(token);
  return std::make_unique<GetSpecifiedTasksCommand>(
      std::vector<TaskId>{task_id_});
}

template <>
std::unique_ptr<Command> AddLabelStep::HandleStage<0>(Context &ctx) {
  if (!ctx.solid_tasks &&
      ctx.status == ModelController::Status::kNotPresentId) {
    return ReportError(Strings::NotPresentId(std::to_string(task_id_.id())));
  }

  auto found =
      std::find_if(ctx.solid_tasks->cbegin(), ctx.solid_tasks->cend(),
                   [this](const auto &i) { return i.task_id() == task_id_; });
  if (found == ctx.solid_tasks->cend()) {
    return ReportError(Strings::NotPresentId(std::to_string(task_id_.id())));
  }

  io_facility_->Print(Strings::YouAreGoingTo("add label to"));
  auto task = found->task();
  io_facility_->Print(Strings::ShowSolidTask(*found));

  io_facility_->Print(Strings::ProceedTo("add label"));
  std::string input = io_facility_->GetLine();
  auto confirm = validator_->ParseConfirmation(input);

  if (!confirm) {
    io_facility_->Print(Strings::kOkayITreatItAsNo);
    return std::make_unique<VoidCommand>();
  }

  if (*confirm == ConfirmationResult::kNo) {
    return std::make_unique<VoidCommand>();
  }

  return std::make_unique<AddLabelCommand>(task_id_, label_);
}

void AddLabelStep::ChangeStep(std::shared_ptr<Step> &active_step) {
  if (stage_ == 0) {
    active_step = std::make_shared<PromptStep>(validator_, io_facility_,
                                               small_step_factory_);
  }
}

std::unique_ptr<Command> AddLabelStep::ReportError(std::string str) {
  stage_ = 0;
  io_facility_->Print(str);
  return std::make_unique<VoidCommand>();
}
}  // namespace task_manager