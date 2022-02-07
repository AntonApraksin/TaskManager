#include "CreateFilledTaskContext.h"

#include "interactor/small_steps/TaskContext.h"
#include "interactor/small_steps/TaskInitializerSmallStep.h"
#include "utils/TaskBuilder.h"

namespace task_manager {
TaskContext CreateFilledTaskContext(ISmallStepFactory& small_step_factory,
                                    const TaskBuilder& task_builder) {
  TaskContext task_context;
  task_context.PushState(
      std::make_shared<DefaultTaskInitializerSmallStep>(task_builder));
  task_context.PushState(small_step_factory.GetReadTitleSmallStep());
  task_context.PushState(small_step_factory.GetReadDateSmallStep());
  task_context.PushState(small_step_factory.GetReadPrioritySmallStep());
  task_context.PushState(small_step_factory.GetReadStateSmallStep());
  return task_context;
}
}  // namespace task_manager