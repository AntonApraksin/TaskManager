#include "interactor/state_machine/commands/Commands.h"

namespace task_manager
{
CommandResult GetAllTasksCommand::execute(ModelController& model_controller)
{
    CommandResult command_result;
    auto result = model_controller.GetAllSolidTasks();
    if (result)
    {
        command_result.solid_tasks = result.AccessResult();
    }
    command_result.status = result.GetStatus();
    return command_result;
}
}  // namespace task_manager
