#include <iostream>

#include "repl/view/step/iostream_step/IostreamGeneralFunctional.h"
#include "repl/view/step/iostream_step/IostreamStep.h"

StepResult IostreamShowAllTasksREPLState::Run() {
  for (const auto &i : task_storage_.get().ShowStorage()) {
    ShowTaskWithId(*i.second, i.first);
    ShowNestedMap(i.second, 3);
  }
  return {{}, {}};
}