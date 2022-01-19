#include "interactor/state_machine/interactor_steps/AddStep.h"

#include <google/protobuf/util/time_util.h>

#include "interactor/io_facility/Strings.h"
#include "interactor/small_steps/ISmallStepFactory.h"
#include "interactor/small_steps/TaskContext.h"
#include "interactor/small_steps/TaskInitializerSmallStep.h"
#include "interactor/state_machine/interactor_steps/PromptStep.h"
#include "utils/TaskIdUtils.h"

namespace task_manager {
std::unique_ptr<Command> AddStep::execute(Context ctx) {
  --stage_;
  if (stage_ == 2) {
    return HandleStage<2>(ctx);
  }
  if (stage_ == 1) {
    return HandleStage<1>(ctx);
  }
  if (stage_ == 0) {
    return HandleStage<0>(ctx);
  }
  std::terminate();
}

template <>
std::unique_ptr<Command> AddStep::HandleStage<2>(Context&) {
  if (arg_.empty()) {
    return std::make_unique<VoidCommand>();
  }
  auto token = validator_->ConsumeOneTokenFrom(arg_);
  auto add_to = validator_->ParseInt(token);
  if (arg_.empty()) {
    if (add_to) {
      task_id_ = CreateTaskId(*add_to);
      return std::make_unique<GetSpecifiedTasksCommand>(
          std::vector<TaskId>{*task_id_});
    }
    return ReportError(Strings::InvalidId(token));
  }
  return ReportError(Strings::kMultipleArgumentAreNotAllowed);
}

template <>
std::unique_ptr<Command> AddStep::HandleStage<1>(Context& ctx) {
  if (task_id_) {
    return HandleAddSubTask(ctx);
  } else {
    return HandleAddTask(ctx);
  }
}

template <>
std::unique_ptr<Command> AddStep::HandleStage<0>(Context& ctx) {
  if (ctx.task_id) {
    io_facility_->Print(Strings::ShowId(std::to_string(ctx.task_id->id())));
  }
  // TODO: Show error?
  return std::make_unique<VoidCommand>();
}

std::unique_ptr<Command> AddStep::HandleAddTask(Context&) {
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
    stage_ = 0;
    return std::make_unique<VoidCommand>();
  }
  if (*confirm == ConfirmationResult::kNo) {
    stage_ = 0;
    return std::make_unique<VoidCommand>();
  }
  return std::make_unique<AddTaskCommand>(
      sub_context.GetTaskBuilder().GetTask());
}

std::unique_ptr<Command> AddStep::HandleAddSubTask(Context& ctx) {
  if (!ctx.solid_tasks) {
    return ReportError(Strings::NotPresentId(std::to_string(task_id_->id())));
  }
  auto found =
      std::find_if(ctx.solid_tasks->cbegin(), ctx.solid_tasks->cend(),
                   [this](const auto& i) { return i.task_id() == *task_id_; });
  if (found == ctx.solid_tasks->cend()) {
    return ReportError(Strings::NotPresentId(std::to_string(task_id_->id())));
  }
  io_facility_->Print(Strings::kAddSubtaskTo);
  auto& task = found->task();
  io_facility_->Print(Strings::ShowSolidTask(*found));
  TaskContext sub_context;
  sub_context.PushState(
      std::make_shared<DefaultTaskInitializerSmallStep>(TaskBuilder{
          std::nullopt, task.due_date(), task.priority(), task.progress()}));
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
    stage_ = 0;
    return std::make_unique<VoidCommand>();
  }
  if (*confirm == ConfirmationResult::kNo) {
    stage_ = 0;
    return std::make_unique<VoidCommand>();
  }
  return std::make_unique<AddSubtaskCommand>(
      *task_id_, sub_context.GetTaskBuilder().GetTask());
}

void AddStep::ChangeStep(std::shared_ptr<Step>& active_step) {
  if (stage_ == 0) {
    active_step = std::make_shared<PromptStep>(validator_, io_facility_,
                                               small_step_factory_);
  }
}

std::unique_ptr<Command> AddStep::ReportError(std::string str) {
  stage_ = 0;
  io_facility_->Print(str);
  return std::make_unique<VoidCommand>();
}

}  // namespace task_manager
