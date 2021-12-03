#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMGENERALFUNCTIONAL_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMGENERALFUNCTIONAL_H_

#include "model/id/TaskId.h"
#include "model/task/Task.h"
#include "model/task_wrapper/TaskWrapper.h"

void ShowTask(const Task&, int = 1);
void ShowTaskWithId(const Task&, TaskId, int = 1);

inline const char* to_string(Task::Priority priority);
inline const char* to_string(Task::State state);

void ShowNestedMap(const TaskWrapper& task_wrapper, int nest);

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMGENERALFUNCTIONAL_H_
