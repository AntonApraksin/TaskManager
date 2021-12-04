#include "repl/view/small_step/TaskContext.h"
#include "repl/view/small_step/task_initializer_small_step/TaskInitializerSmallStep.h"

DefaultTaskInitializerSmallStep::DefaultTaskInitializerSmallStep(
    const TaskBuilder &task_builder)
    : task_builder_(task_builder) {}

void DefaultTaskInitializerSmallStep::Execute(TaskContext &ctx) {
  ctx.GetTaskBuilder() = task_builder_;
  ctx.PopState();
}