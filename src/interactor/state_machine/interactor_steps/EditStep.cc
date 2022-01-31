#include "interactor/state_machine/interactor_steps/EditStep.h"

#include <google/protobuf/util/time_util.h>

#include "interactor/io_facility/Strings.h"
#include "interactor/small_steps/CreateFilledTaskContext.h"
#include "interactor/small_steps/TaskContext.h"
#include "interactor/small_steps/TaskInitializerSmallStep.h"
#include "interactor/state_machine/interactor_steps/FinalizeStep.h"
#include "interactor/state_machine/interactor_steps/utils/IoFacilityAndValidatorUtils.h"
#include "interactor/state_machine/interactor_steps/utils/ValidatorUtils.h"
#include "utils/Functions.h"

namespace task_manager {
std::unique_ptr<Command> EditStep::execute(StepParameter &param) {
  using google::protobuf::util::TimeUtil;
  param.ctx.event = StepEvent::kNothing;

  if (arg_.empty()) {
    return ReportError(Strings::kRequiredId);
  }

  auto task_id = ConsumeTaskIdFromString(*validator_, arg_);
  if (!arg_.empty()) {
    return ReportError(Strings::kMultipleArgumentDoesNotSupported);
  }
  if (!task_id) {
    return ReportError(Strings::kInvalidId);
  }

  auto found = FindSolidTaskById(param.cache, *task_id);
  TaskContext task_context;
  if (found) {
    io_facility_->Print(Strings::YouAreGoingTo("edit"));
    io_facility_->Print(Strings::ShowSolidTask(*found));

    auto &task = found->task();
    TaskBuilder task_builder{task.title(), task.due_date(), task.priority(),
                             task.progress()};
    task_context = CreateFilledTaskContext(*small_step_factory_, task_builder);

  } else {
    TaskBuilder task_builder{std::nullopt,
                             TimeUtil::TimeTToTimestamp(std::time(nullptr)),
                             Task::kLow, Task::kUncompleted};
    task_context = CreateFilledTaskContext(*small_step_factory_, task_builder);
  }

  task_context.Run();

  auto confirm = ReadConfirmation(*io_facility_, *validator_, "edit");
  if (!confirm) {
    io_facility_->Print(Strings::kOkayITreatItAsNo);
    return std::make_unique<VoidCommand>();
  }
  if (*confirm == ConfirmationResult::kNo) {
    return std::make_unique<VoidCommand>();
  }

  param.cache.clear();

  auto new_task = task_context.GetTaskBuilder().GetTask();
  return std::make_unique<EditTaskCommand>(*task_id, new_task);
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
