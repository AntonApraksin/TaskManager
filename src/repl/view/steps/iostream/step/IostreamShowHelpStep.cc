#include <sstream>

#include "repl/view/steps/iostream/step/IostreamStep.h"

IostreamShowHelpStep::IostreamShowHelpStep(
    const std::shared_ptr<IIoFacility>& io_facility)
    : IostreamStep(io_facility) {}

StepResult IostreamShowHelpStep::Run() {
  std::stringstream ss;
  ss << "Usage:\n"
     << "  add(a) [id]\n"
     << "    Add a new task.\n"
     << "  edit(e) id\n"
     << "    Edit a task with given id.\n"
     << "  delete(d) id [id...]\n"
     << "    Delete a task with the given id.\n"
     << "  complete(c) id [id...]\n"
     << "    Complete a task with the given id.\n"
     << "  show(s)\n"
     << "    Show all tasks.\n"
     << "  quit(q)\n"
     << "    Quit.\n";
  io_facility_->Print(ss.str());
  return {{}, {}};
}