#include "interactor/state_machine/commands/Commands.h"

namespace task_manager
{
CommandResult LoadTasksCommand::execute(ModelController& model_controller)
{
    CommandResult command_result;
    auto result = model_controller.Load();
    command_result.status = result.GetStatus();
    return command_result;
}
}  // namespace task_manager
