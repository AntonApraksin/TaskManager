#include "Commands.h"

namespace task_manager
{
DeleteLabelCommand::DeleteLabelCommand(TaskId task_id, Label label)
    : task_id_(std::move(task_id)), label_(std::move(label)) {}

CommandResult DeleteLabelCommand::execute(ModelController& model_controller)
{
    CommandResult ret;
    auto result =
        model_controller.DeleteLabel(std::move(task_id_), std::move(label_));
    ret.status = result.GetStatus();
    return ret;
}
}  // namespace task_manager
