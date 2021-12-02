#include "repl/view/step/iostream_step/IostreamStep.h"

#include <iostream>

StepResult IostreamShowHelpREPLState::Run() {
  std::cout << "Usage:\n"
            << "  add(a)\n"
            << "    Add a new task.\n"
            << "  edit(ed) id\n"
            << "    Edit a task with given id.\n"
            << "  delete(d) id\n"
            << "    Delete a task with the given id.\n"
            << "  complete(c) id\n"
            << "    Complete a task with the given id.\n"
            << "  show(s)\n"
            << "    Show all uncompleted tasks.\n"
            << "  exit(ex)\n"
            << "    Exit.\n";
  return {{}, {}};
}