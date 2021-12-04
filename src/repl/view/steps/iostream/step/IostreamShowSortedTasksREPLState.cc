#include <iostream>

#include "IostreamStep.h"
#include "repl/view/steps/iostream/IostreamGeneralFunctional.h"

StepResult IostreamShowSortedTasksREPLState::Run() {
  for (const auto &i : tasks_) {
    ShowTaskWithId(i.second, i.first);
  }
  return {{}, {}};
}