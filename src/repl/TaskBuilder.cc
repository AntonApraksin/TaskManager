#include "TaskBuilder.h"

Task TaskBuilder::GetTask() {
  return *Task::Create(*title_, *priority_, *date_, *state_);
}
