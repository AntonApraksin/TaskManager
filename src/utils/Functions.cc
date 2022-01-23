#include "Functions.h"

#include "utils/TaskIdUtils.h"

namespace task_manager {
std::pair<SolidTasks::iterator, SolidTasks::iterator> GetTreeFromVector(
    SolidTasks& solid_tasks, TaskId id) {
  std::pair<SolidTasks::iterator, SolidTasks::iterator> ret{solid_tasks.end(),
                                                            solid_tasks.end()};
  std::vector<TaskId> id_stack;
  for (auto i = solid_tasks.begin(), end = solid_tasks.end(); i != end; ++i) {
    if (i->task_id() == id) {
      id_stack.push_back(i->task_id());
      ret.first = i;
      continue;
    } else if (id_stack.empty() && ret.first != solid_tasks.end()) {
      ret.second = i;
      return ret;
    } else if (ret.first != solid_tasks.end()) {
      for (; !id_stack.empty() && id_stack.back() != i->parent_id();) {
        id_stack.pop_back();
      }
      if (!id_stack.empty()) {
        id_stack.push_back(i->task_id());
      } else {
        ret.second = i;
        return ret;
      }
    }
  }
  return ret;
}
}  // namespace task_manager
