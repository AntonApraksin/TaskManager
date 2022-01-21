#include "interactor/state_machine/interactor_steps/CompleteStep.h"

#include "interactor/io_facility/Strings.h"
#include "interactor/state_machine/interactor_steps/FinalizeStep.h"
#include "utils/TaskIdUtils.h"

namespace task_manager {
std::unique_ptr<Command> CompleteStep::execute(StepParameter &param) {
  if (arg_.empty()) {
    return ReportError(Strings::kRequiredId);
  }
  std::string token;
  auto to_complete =
      validator_->ParseInt(validator_->ConsumeOneTokenFrom(arg_));
  if (!to_complete) {
    return ReportError(Strings::InvalidId(token));
  }
  task_id_.set_id(*to_complete);

  auto found =
      std::find_if(param.cache.begin(), param.cache.end(),
                   [this](const auto &i) { return i.task_id() == task_id_; });
  if (found != param.cache.end()) {
    io_facility_->Print(Strings::YouAreGoingTo("complete"));
    io_facility_->Print(Strings::ShowSolidTask(*found));
  }

  io_facility_->Print(Strings::ProceedTo("complete"));
  std::string input = io_facility_->GetLine();
  auto confirm = validator_->ParseConfirmation(input);
  if (!confirm) {
    io_facility_->Print(Strings::kOkayITreatItAsNo);
    return std::make_unique<VoidCommand>();
  }
  if (*confirm == ConfirmationResult::kNo) {
    return std::make_unique<VoidCommand>();
  }
  return std::make_unique<CompleteTasksCommand>(std::vector<TaskId>{task_id_});
}

void CompleteStep::ChangeStep(std::shared_ptr<Step> &active_step) {
  active_step = std::make_shared<FinalizeStep>(validator_, io_facility_,
                                               small_step_factory_);
}

std::unique_ptr<Command> CompleteStep::ReportError(std::string str) {
  io_facility_->Print(str);
  return std::make_unique<VoidCommand>();
}
}  // namespace task_manager