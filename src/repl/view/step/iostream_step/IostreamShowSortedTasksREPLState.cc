#include "repl/view/step/iostream_step/IostreamStep.h"

#include <iostream>

StepResult IostreamShowSortedTasksREPLState::Run() {
  for(const auto &i : tasks_){
    std::cout << i.first.GetId() << ": [" << to_string(i.second.GetPriority()) << "] "
              << "(" << to_string(i.second.GetState()) << ") "
              << i.second.GetTitle() << "\n";  // TODO: Display date, add children
  }
  return {{}, {}};
}