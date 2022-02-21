#include <boost/log/attributes/named_scope.hpp>

#include "interactor/state_machine/commands/Commands.h"
#include "logging/DefaultLogFacility.h"

namespace task_manager
{
AddTaskCommand::AddTaskCommand(Task task) : task_(std::move(task)) {}

CommandResult AddTaskCommand::execute(ModelController& model_controller)
{
    BOOST_LOG_NAMED_SCOPE("AddTaskCommand::execute");
    auto& logger = logging::GetDefaultLogger();

    BOOST_LOG_SEV(logger, logging::severinity::info)
        << "Adding task: " << task_.DebugString();

    CommandResult command_result;
    auto result = model_controller.AddTask(task_);
    if (result)
    {
        command_result.task_id = result.AccessResult();

        BOOST_LOG_SEV(logger, logging::severinity::info)
            << "Ok. New TaskId: " << result.AccessResult().DebugString();
    }
    else
    {
        BOOST_LOG_SEV(logger, logging::severinity::info)
            << "Error: " << static_cast<int>(result.GetStatus());
    }
    command_result.status = result.GetStatus();
    return command_result;
}
}  // namespace task_manager
