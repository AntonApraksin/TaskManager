#ifndef TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMGENERALFUNCTIONAL_H_
#define TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMGENERALFUNCTIONAL_H_

#include "model/id/TaskId.h"
#include "model/task/Task.h"
#include "model/task_wrapper/TaskWrapper.h"
#include "repl/io_facility/IIoFacility.h"

void ShowTask(IIoFacility& io, const Task&);
void ShowTask(IIoFacility& io, const Task&, int);
void ShowTaskWithId(IIoFacility& io, const Task&, TaskId);
void ShowTaskWithId(IIoFacility& io, const Task&, TaskId, int);

const char* to_string(Task::Priority priority);
const char* to_string(Task::State state);

void ShowNestedMap(IIoFacility& io, const TaskWrapper& task_wrapper, int nest);

std::string PrintAndGet(IIoFacility& io, const std::string& str);

#endif  // TASKMANAGER_SRC_REPL_VIEW_STEP_IOSTREAM_STEP_IOSTREAMGENERALFUNCTIONAL_H_
