#include "AddLabelStep.h"

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/FinalizeStep.h"
#include "utils/TaskIdUtils.h"

namespace task_manager {

std::unique_ptr<Command> AddLabelStep::execute(StepParameter &param) {
  param.ctx.event = StepEvent::kNothing;
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

  auto found =
      std::find_if(param.cache.begin(), param.cache.end(),
                   [this](const auto &i) { return i.task_id() == task_id_; });
  if (found != param.cache.end()) {
    io_facility_->Print(Strings::YouAreGoingTo("add label to"));
    io_facility_->Print(Strings::ShowSolidTask(*found));
  }

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

  if (found != param.cache.end()) {
    auto label_ptr = found->mutable_task()->add_labels();
    label_ptr->set_name(label_.name());
  }
  return std::make_unique<AddLabelCommand>(task_id_, label_);
}

void AddLabelStep::ChangeStep(std::shared_ptr<Step> &active_step) {
  active_step = std::make_shared<FinalizeStep>(validator_, io_facility_,
                                               small_step_factory_);
}

std::unique_ptr<Command> AddLabelStep::ReportError(std::string str) {
  io_facility_->Print(str);
  return std::make_unique<VoidCommand>();
}
}  // namespace task_manager