#include "TaskInitializerSmallStep.h"
#include "repl/view/steps/TaskContext.h"

DefaultTaskInitializerSmallStep::DefaultTaskInitializerSmallStep(
    const TaskBuilder &task_builder)
    : task_builder_(task_builder) {}

void DefaultTaskInitializerSmallStep::Execute(TaskContext &ctx) {
  ctx.GetTaskBuilder() = task_builder_;
  ctx.PopState();
}