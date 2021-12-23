#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_STEPRESULT_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_STEPRESULT_H_

#include "Task.pb.h"
#include "repl/validator/IValidator.h"

namespace task_manager {
struct StepResult {
  std::optional<ConfirmationResult> confirmation_result;
  std::optional<Task> task;
};
}  // namespace task_manager

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_STEPRESULT_H_
