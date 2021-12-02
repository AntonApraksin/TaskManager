#include "repl/view/step/iostream_step/IostreamStep.h"

#include <iostream>

inline void ShowNestedMap(const TaskWrapper& task_wrapper, int nest)
{
  std::string indent(nest*2, ' ');
  std::cout << " [" << to_string(task_wrapper->GetState()) << "] "
            << "(" << to_string(task_wrapper->GetPriority()) << ") "
            << task_wrapper->GetTitle() << "\n";  // TODO: Display date
            for(const auto &i : task_wrapper.ShowStorage()){
              std::cout << indent << i.first.GetId();
              ShowNestedMap(i.second, nest+1);
            }
}

StepResult IostreamShowAllTasksREPLState::Run() {
  for(const auto &i : task_storage_.get().ShowStorage()) {
    std::cout << ' ' << i.first.GetId();
    ShowNestedMap(i.second, 1);
  }
    return {{}, {}};
  }