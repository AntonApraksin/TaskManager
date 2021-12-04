#include <iostream>

#include "IostreamStep.h"
#include "repl/view/steps/iostream/IostreamGeneralFunctional.h"

StepResult IostreamShowNTasksREPLState::Run() {
  for (const auto &i : task_wrappers_) {
    ShowNestedMap(i, 2);
  }
  return {{}, {}};
}