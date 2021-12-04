#ifndef TASKMANAGER_SRC_REPL_VIEW_SMALL_STEP_TASK_INITIALIZER_SMALL_STEP_TASKINITIALIZERSMALLSTEP_H_
#define TASKMANAGER_SRC_REPL_VIEW_SMALL_STEP_TASK_INITIALIZER_SMALL_STEP_TASKINITIALIZERSMALLSTEP_H_

#include "repl/TaskBuilder.h"
#include "repl/view/steps/ISmallStep.h"

class DefaultTaskInitializerSmallStep : public ISmallStep {
 public:
  explicit DefaultTaskInitializerSmallStep(const TaskBuilder&);
  void Execute(TaskContext& ctx) override;

 private:
  TaskBuilder task_builder_;
};

#endif  // TASKMANAGER_SRC_REPL_VIEW_SMALL_STEP_TASK_INITIALIZER_SMALL_STEP_TASKINITIALIZERSMALLSTEP_H_
