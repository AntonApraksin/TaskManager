#include "TaskBuilder.h"

namespace task_manager
{
Task TaskBuilder::GetTask()
{
    return *CreateTask(*title_, *date_, *priority_, *progress_);
}
}  // namespace task_manager
