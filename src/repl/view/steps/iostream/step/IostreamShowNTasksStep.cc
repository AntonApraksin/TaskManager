#include <iostream>

#include "IostreamStep.h"
#include "repl/view/steps/iostream/IostreamGeneralFunctional.h"

StepResult IostreamShowNTasksStep::Run() {
  for (const auto &i : task_wrappers_) {
    ShowNestedMap(i, 2);
  }
  return {{}, {}};
}