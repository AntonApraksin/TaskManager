#include <iostream>

#include "repl/view/step/iostream_step/IostreamGeneralFunctional.h"
#include "repl/view/step/iostream_step/IostreamStep.h"

StepResult IostreamShowSortedTasksREPLState::Run() {
  for (const auto &i : tasks_) {
    ShowTaskWithId(i.second, i.first);
  }
  return {{}, {}};
}