#include <iostream>

#include "repl/view/step/iostream_step/IostreamGeneralFunctional.h"
#include "repl/view/step/iostream_step/IostreamStep.h"

StepResult IostreamShowNTasksREPLState::Run() {
  for (const auto &i : task_wrappers_) {
    ShowNestedMap(i, 2);
  }
  return {{}, {}};
}