#include <google/protobuf/util/time_util.h>

#include "repl/io_facility/Strings.h"
#include "repl/state_machine/repl_steps/ReplSteps.h"
#include "repl/view/steps/ISmallStepFactory.h"
#include "repl/view/steps/TaskContext.h"
#include "repl/view/steps/TaskInitializerSmallStep.h"
#include "utils/TaskIdUtils.h"

namespace task_manager {
std::unique_ptr<Command> AddReplStep::execute(Context ctx) {
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
std::unique_ptr<Command> AddReplStep::HandleStage<2>(Context&) {
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
  return ReportError(Strings::kMultipleArgumentDoesNotSupported);
}

template <>
std::unique_ptr<Command> AddReplStep::HandleStage<1>(Context& ctx) {
  if (task_id_) {
    return HandleAddSubTask(ctx);
  } else {
    return HandleAddTask(ctx);
  }
}

template <>
std::unique_ptr<Command> AddReplStep::HandleStage<0>(Context& ctx) {
  if (ctx.task_id) {
    io_facility_->Print(Strings::ShowId(std::to_string(ctx.task_id->id())));
  }
  // TODO: Show error?
  return std::make_unique<VoidCommand>();
}

std::unique_ptr<Command> AddReplStep::HandleAddTask(Context&) {
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
  return std::make_unique<AddTaskCommand>(
      sub_context.GetTaskBuilder().GetTask());
}

std::unique_ptr<Command> AddReplStep::HandleAddSubTask(Context& ctx) {
  if (!ctx.solid_tasks) {
    return ReportError(Strings::NotPresentId(std::to_string(task_id_->id())));
  }
  io_facility_->Print(Strings::kAddSubtaskTo);
  auto& task = ctx.solid_tasks->at(0).task();
  io_facility_->Print(Strings::ShowSolidTask(ctx.solid_tasks->at(0)));
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
    return std::make_unique<VoidCommand>();
  }
  if (*confirm == ConfirmationResult::kNo) {
    return std::make_unique<VoidCommand>();
  }
  return std::make_unique<AddSubtaskCommand>(
      *task_id_, sub_context.GetTaskBuilder().GetTask());
}

void AddReplStep::ChangeStep(std::shared_ptr<ReplSteps>& active_step) {
  if (stage_ == 0) {
    active_step = std::make_shared<PromptReplStep>(validator_, io_facility_,
                                                   small_step_factory_);
  }
}

std::unique_ptr<Command> AddReplStep::ReportError(std::string str) {
  stage_ = 0;
  io_facility_->Print(str);
  return std::make_unique<VoidCommand>();
}

}  // namespace task_manager
