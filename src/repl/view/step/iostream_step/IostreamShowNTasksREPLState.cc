#include "repl/view/step/iostream_step/IostreamStep.h"

#include <iostream>

void ShowNestedMap(const TaskWrapper& task_wrapper, int nest)
{
  std::string indent(nest*2, ' ');
  std::cout << "[" << to_string(task_wrapper->GetState()) << "] "
            << "(" << to_string(task_wrapper->GetState()) << ") "
            << task_wrapper->GetTitle() << "\n";  // TODO: Display date
  for(const auto &i : task_wrapper.ShowStorage()){
    std::cout << indent << i.first.GetId();
    ShowNestedMap(i.second, nest+1);
  }
}

StepResult IostreamShowNTasksREPLState::Run() {

  for(const auto &i : task_wrappers_){
    std::cout << "[" << to_string(i.get()->GetState()) << "] "
              << "(" << to_string(i.get()->GetState()) << ") "
              << i.get()->GetTitle() << "\n";  // TODO: Display date, add children
  }
  return {{}, {}};
}