#include "StateMachineController.h"

#include "interactor/state_machine/commands/CommandResult.h"

namespace task_manager
{

StepEvent MCStatusToStepEvent(ModelController::Status status)
{
    switch (status)
    {
        case ModelController::Status::kOk:return StepEvent::kNothing;

        case ModelController::Status::kSaveFailure:return StepEvent::kSaveFailure;

        case ModelController::Status::kLoadFailure:return StepEvent::kLoadFailure;

        case ModelController::Status::kNotPresentId:return StepEvent::kNotPresentId;

        case ModelController::Status::kNotPresentLabel:return StepEvent::kNotPresentLabel;
    }
}

StateMachineController::StateMachineController(
    std::shared_ptr<ModelController> model_controller,
    std::unique_ptr<StateMachine> state_machine)
    : model_controller_(std::move(model_controller)),
      state_machine_(std::move(state_machine)) {}

void StateMachineController::Run()
{
    Context ctx;
    ctx.event = StepEvent::kNothing;
    CommandResult command_result;
    auto command = state_machine_->execute(ctx);
    for (; command ;)
    {
        command_result = command->execute(*model_controller_);
        ctx.event = MCStatusToStepEvent(command_result.status);
        ctx.task_id = std::move(command_result.task_id);
        ctx.solid_tasks = std::move(command_result.solid_tasks);
        command = state_machine_->execute(ctx);
    }
}
}  // namespace task_manager
