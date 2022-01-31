#include "interactor/state_machine/interactor_steps/AddStep.h"

#include <google/protobuf/util/time_util.h>

#include "interactor/io_facility/Strings.h"
#include "interactor/small_steps/ISmallStepFactory.h"
#include "interactor/small_steps/TaskContext.h"
#include "interactor/small_steps/TaskInitializerSmallStep.h"
#include "interactor/state_machine/interactor_steps/FinalizeStep.h"
#include "utils/TaskIdUtils.h"

namespace task_manager {
std::unique_ptr<Command> AddStep::execute(StepParameter& param) {
  param.ctx.event = StepEvent::kNothing;
  if (!arg_.empty()) {
    auto token = validator_->ConsumeOneTokenFrom(arg_);
    auto add_to = validator_->ParseInt(token);
    if (arg_.empty()) {
      if (add_to) {
        task_id_ = CreateTaskId(*add_to);
        return HandleAddSubTask(param);
      }
      return ReportError(Strings::InvalidId(token));
    }
    return ReportError(Strings::kMultipleArgumentDoesNotSupported);
  }
  return HandleAddTask(param);
}

std::unique_ptr<Command> AddStep::HandleAddTask(StepParameter& param) {
  TaskContext sub_context;
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

  io_facility_->Print(Strings::ProceedTo("add"));
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
  param.ctx.event = StepEvent::kShowId;

  param.cache.clear();
  return std::make_unique<AddTaskCommand>(new_task);
}

std::unique_ptr<Command> AddStep::HandleAddSubTask(StepParameter& param) {
  auto found =
      std::find_if(param.cache.begin(), param.cache.end(),
                   [this](const auto& i) { return i.task_id() == task_id_; });
  TaskContext sub_context;
  if (found != param.cache.end()) {
    io_facility_->Print(Strings::kAddSubtaskTo);
    io_facility_->Print(Strings::ShowSolidTask(*found));
    auto& task = found->task();
    sub_context.PushState(
        std::make_shared<DefaultTaskInitializerSmallStep>(TaskBuilder{
            std::nullopt, task.due_date(), task.priority(), task.progress()}));
  } else {
    sub_context.PushState(std::make_shared<DefaultTaskInitializerSmallStep>(
        TaskBuilder{std::nullopt,
                    google::protobuf::util::TimeUtil::TimeTToTimestamp(
                        std::time(nullptr)),
                    Task::kLow, Task::kUncompleted}));
  }
  sub_context.PushState(small_step_factory_->GetReadTitleSmallStep());
  sub_context.PushState(small_step_factory_->GetReadDateSmallStep());
  sub_context.PushState(small_step_factory_->GetReadPrioritySmallStep());
  sub_context.PushState(small_step_factory_->GetReadStateSmallStep());
  sub_context.Run();

  io_facility_->Print(Strings::ProceedTo("add"));
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
  param.ctx.event = StepEvent::kShowId;

  param.cache.clear();
  return std::make_unique<AddSubtaskCommand>(*task_id_, new_task);
}

void AddStep::ChangeStep(std::shared_ptr<Step>& active_step) {
  active_step = std::make_shared<FinalizeStep>(validator_, io_facility_,
                                               small_step_factory_);
}

std::unique_ptr<Command> AddStep::ReportError(std::string str) {
  io_facility_->Print(str);
  return std::make_unique<VoidCommand>();
}

}  // namespace task_manager
