#include "interactor/state_machine/commands/Commands.h"

namespace task_manager
{
CommandResult VoidCommand::execute(ModelController&)
{
    CommandResult command_result;
    command_result.status = ModelController::Status::kOk;
    return command_result;
}
}  // namespace task_manager
