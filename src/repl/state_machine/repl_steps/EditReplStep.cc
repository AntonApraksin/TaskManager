#include "repl/state_machine/repl_steps/EditReplStep.h"

#include "repl/io_facility/Strings.h"
#include "repl/small_steps/ISmallStepFactory.h"
#include "repl/small_steps/TaskContext.h"
#include "repl/small_steps/TaskInitializerSmallStep.h"
#include "repl/state_machine/repl_steps/PromptReplStep.h"
#include "utils/TaskIdUtils.h"

namespace task_manager {
std::unique_ptr<Command> EditReplStep::execute(Context ctx) {
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
std::unique_ptr<Command> EditReplStep::HandleStage<1>(Context &) {
  if (arg_.empty()) {
    return ReportError(Strings::kRequiredId);
  }
  auto token = validator_->ConsumeOneTokenFrom(arg_);
  auto to_edit = validator_->ParseInt(token);
  if (arg_.empty()) {
    if (to_edit) {
      task_id_.set_id(*to_edit);
      return std::make_unique<GetSpecifiedTasksCommand>(
          std::vector<TaskId>{task_id_});
    } else {
      return ReportError(Strings::InvalidId(token));
    }
  }
  return ReportError(Strings::kMultipleArgumentDoesNotSupported);
}

template <>
std::unique_ptr<Command> EditReplStep::HandleStage<0>(Context &ctx) {
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
  io_facility_->Print(Strings::kYouAreGoingToEdit);
  auto task = found->task();
  io_facility_->Print(Strings::ShowSolidTask(*found));
  TaskContext sub_context;
  sub_context.PushState(
      std::make_shared<DefaultTaskInitializerSmallStep>(TaskBuilder{
          task.title(), task.due_date(), task.priority(), task.progress()}));
  sub_context.PushState(small_step_factory_->GetReadTitleSmallStep());
  sub_context.PushState(small_step_factory_->GetReadDateSmallStep());
  sub_context.PushState(small_step_factory_->GetReadPrioritySmallStep());
  sub_context.PushState(small_step_factory_->GetReadStateSmallStep());
  sub_context.Run();

  io_facility_->Print(Strings::ProceedTo("edit"));
  std::string input = io_facility_->GetLine();
  auto confirm = validator_->ParseConfirmation(input);
  if (!confirm) {
    io_facility_->Print(Strings::kOkayITreatItAsNo);
    return std::make_unique<VoidCommand>();
  }
  if (*confirm == ConfirmationResult::kNo) {
    return std::make_unique<VoidCommand>();
  }
  return std::make_unique<EditTaskCommand>(
      task_id_, sub_context.GetTaskBuilder().GetTask());
}

std::unique_ptr<Command> EditReplStep::ReportError(std::string str) {
  stage_ = 0;
  io_facility_->Print(str);
  return std::make_unique<VoidCommand>();
}

void EditReplStep::ChangeStep(std::shared_ptr<ReplStep> &active_step) {
  if (stage_ == 0) {
    active_step = std::make_shared<PromptReplStep>(validator_, io_facility_,
                                                   small_step_factory_);
  }
}

}  // namespace task_manager
