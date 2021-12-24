#include <google/protobuf/util/time_util.h>

#include "DefaultStep.h"
#include "repl/view/steps/ISmallStepFactory.h"
#include "repl/view/steps/Strings.h"
#include "repl/view/steps/TaskContext.h"
#include "repl/view/steps/TaskInitializerSmallStep.h"

namespace task_manager {
DefaultAddTaskStep::DefaultAddTaskStep(
    const std::shared_ptr<IIoFacility>& io_facility,
    const std::shared_ptr<ISmallStepFactory>& small_step_factory,
    const std::shared_ptr<IValidator>& validator)
    : DefaultStep(io_facility),
      DefaultWithSmallStepStep(small_step_factory),
      DefaultWithValidatorStep(validator) {}

StepResult DefaultAddTaskStep::Run() {
  TaskContext sub_context;
  if (solid_task_) {
    io_facility_->Print(Strings::kAddSubtaskTo);
    io_facility_->Print(Strings::ShowSolidTask(*solid_task_));
    sub_context.PushState(std::make_shared<DefaultTaskInitializerSmallStep>(
        TaskBuilder{/*.title = */ std::nullopt,
                    /*.date_ =*/solid_task_->task().due_date(),
                    /*.priority =*/solid_task_->task().priority(),
                    /*.state =*/solid_task_->task().progress()}));
    solid_task_.reset();
  } else {
    sub_context.PushState(std::make_shared<DefaultTaskInitializerSmallStep>(
        TaskBuilder{/*.title =*/std::nullopt,
                    /*.date_ =*/
                    google::protobuf::util::TimeUtil::TimeTToTimestamp(
                        std::time(nullptr)),
                    /*.priority =*/Task::kLow,
                    /*.progress =*/Task::kUncompleted}));
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
    return StepResult{ConfirmationResult::kNo,
                      sub_context.GetTaskBuilder().GetTask()};
  }
  return {*confirm, sub_context.GetTaskBuilder().GetTask()};
}
}  // namespace task_manager
