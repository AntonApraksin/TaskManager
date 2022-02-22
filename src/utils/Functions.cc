#include "Functions.h"

#include "utils/TaskIdUtils.h"

namespace task_manager
{
SolidTasks GetTreeFromVector(const SolidTasks& solid_tasks, const TaskId& id)
{
    std::pair<SolidTasks::const_iterator, SolidTasks::const_iterator> iters{
        solid_tasks.end(), solid_tasks.end()};
    std::vector<TaskId> id_stack;
    for (auto i = solid_tasks.begin(), end = solid_tasks.end() ; i != end ; ++i)
    {
        if (i->task_id() == id)
        {
            id_stack.push_back(i->task_id());
            iters.first = i;
            continue;
        }
        else if (id_stack.empty() && iters.first != solid_tasks.end())
        {
            iters.second = i;
            break;
        }
        else if (iters.first != solid_tasks.end())
        {
            for (; !id_stack.empty() && id_stack.back() != i->parent_id() ;)
            {
                id_stack.pop_back();
            }
            if (!id_stack.empty())
            {
                id_stack.push_back(i->task_id());
            }
            else
            {
                iters.second = i;
                break;
            }
        }
    }

    SolidTasks ret(iters.first, iters.second);
    if (!ret.empty())
    {
        ret.front().clear_parent_id();
    }
    return SolidTasks(iters.first, iters.second);
}

std::optional<SolidTask> FindSolidTaskById(const SolidTasks& solid_tasks,
                                           const TaskId& task_id)
{
    auto found = std::find_if(
        solid_tasks.cbegin(), solid_tasks.cend(),
        [&task_id](const auto& i) { return i.task_id() == task_id; });
    if (found == solid_tasks.cend())
    {
        return {};
    }
    return *found;
}
}  // namespace task_manager
