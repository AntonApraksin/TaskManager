#include "TaskInitializerSmallStep.h"

#include "TaskContext.h"

namespace task_manager {
DefaultTaskInitializerSmallStep::DefaultTaskInitializerSmallStep(
    const TaskBuilder &task_builder)
    : task_builder_(task_builder) {}

void DefaultTaskInitializerSmallStep::Execute(TaskContext &ctx) {
  ctx.GetTaskBuilder() = task_builder_;
  ctx.PopState();
}
}  // namespace task_manager