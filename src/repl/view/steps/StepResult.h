#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_STEPRESULT_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_STEPRESULT_H_

#include "model/task/Task.h"
#include "repl/validator/IValidator.h"

struct StepResult {
  std::optional<ConfirmationResult> confirmation_result;
  std::optional<Task> task;
};

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_STEPRESULT_H_
