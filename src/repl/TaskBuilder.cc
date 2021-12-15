#include "TaskBuilder.h"

Task TaskBuilder::GetTask() {
  return *CreateTask(*title_, *date_, *priority_, *progress_);
}
