#include <boost/log/attributes/named_scope.hpp>

#include "interactor/state_machine/commands/Commands.h"
#include "logging/DefaultLogFacility.h"

namespace task_manager
{
GetSpecifiedTasksCommand::GetSpecifiedTasksCommand(std::vector<TaskId> task_ids)
    : task_ids_(std::move(task_ids)) {}

CommandResult GetSpecifiedTasksCommand::execute(
    ModelController& model_controller)
{
    BOOST_LOG_NAMED_SCOPE("GetSpecifiedTasksCommand::execute");
    auto& logger = logging::GetDefaultLogger();

    BOOST_LOG_SEV(logger, logging::severinity::info)
        << "Querying " << task_ids_.size() << " ids";

    CommandResult command_result;
    auto result = model_controller.GetSpecificSolidTasks(std::move(task_ids_));
    if (result)
    {
        command_result.solid_tasks = result.AccessResult();

        BOOST_LOG_SEV(logger, logging::severinity::info)
            << "Ok. Got " << result.AccessResult().size() << " tasks";
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
