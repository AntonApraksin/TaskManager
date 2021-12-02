#include "repl/view/step/iostream_step/IostreamStep.h"

#include <iostream>

StepResult IostreamUnknownCommandsREPLState::Run() {
  std::cout << "Unknown command. Type 'help' to display all available commands.\n";
  return {{}, {}};
}