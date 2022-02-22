#ifndef TASKMANAGER_SRC_REPL_TASKBUILDER_H_
#define TASKMANAGER_SRC_REPL_TASKBUILDER_H_

#include <optional>

#include "Task.pb.h"
#include "utils/TaskUtils.h"

namespace task_manager
{
struct TaskBuilder
{
    Task GetTask();

    std::optional<std::string> title_;
    std::optional<TaskDate_t> date_;
    std::optional<Task::Priority> priority_;
    std::optional<Task::Progress> progress_;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_TASKBUILDER_H_
