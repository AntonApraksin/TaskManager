#include "interactor/state_machine/interactor_steps/EditStep.h"

#include <google/protobuf/util/time_util.h>

#include "interactor/io_facility/Strings.h"
#include "interactor/small_steps/ISmallStepFactory.h"
#include "interactor/small_steps/TaskContext.h"
#include "interactor/small_steps/TaskInitializerSmallStep.h"
#include "interactor/state_machine/interactor_steps/FinalizeStep.h"
#include "utils/TaskIdUtils.h"

namespace task_manager {
std::unique_ptr<Command> EditStep::execute(StepParameter &param) {
  if (arg_.empty()) {
    return ReportError(Strings::kRequiredId);
  }
  std::string token;
  auto to_edit = validator_->ParseInt(validator_->ConsumeOneTokenFrom(arg_));
  if (!to_edit) {
    return ReportError(Strings::InvalidId(token));
  }
  task_id_.set_id(*to_edit);

  auto found =
      std::find_if(param.cache.begin(), param.cache.end(),
                   [this](const auto &i) { return i.task_id() == task_id_; });

  TaskContext sub_context;
  if (found != param.cache.end()) {
    io_facility_->Print(Strings::YouAreGoingTo("edit"));
    io_facility_->Print(Strings::ShowSolidTask(*found));
    auto &task = found->task();
    sub_context.PushState(
        std::make_shared<DefaultTaskInitializerSmallStep>(TaskBuilder{
            task.title(), task.due_date(), task.priority(), task.progress()}));
  }

  sub_context.PushState(std::make_shared<DefaultTaskInitializerSmallStep>(
      TaskBuilder{std::nullopt,
                  google::protobuf::util::TimeUtil::TimeTToTimestamp(
                      std::time(nullptr)),
                  Task::kLow, Task::kUncompleted}));
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

  auto new_task = sub_context.GetTaskBuilder().GetTask();
  if (found != param.cache.end()) {
    found->set_allocated_task(new Task(new_task));
  }
  return std::make_unique<EditTaskCommand>(task_id_, new_task);
}

std::unique_ptr<Command> EditStep::ReportError(std::string str) {
  io_facility_->Print(str);
  return std::make_unique<VoidCommand>();
}

void EditStep::ChangeStep(std::shared_ptr<Step> &active_step) {
  active_step = std::make_shared<FinalizeStep>(validator_, io_facility_,
                                               small_step_factory_);
}

}  // namespace task_manager
