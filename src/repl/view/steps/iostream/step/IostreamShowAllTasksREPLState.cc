#include <iostream>

#include "IostreamStep.h"
#include "repl/view/steps/iostream/IostreamGeneralFunctional.h"

StepResult IostreamShowAllTasksREPLState::Run() {
  for (const auto &i : task_storage_.get().ShowStorage()) {
    ShowTaskWithId(*i.second, i.first);
    ShowNestedMap(i.second, 3);
  }
  return {{}, {}};
}