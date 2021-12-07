#include "View.h"

#include <sstream>

#include "repl/view/steps/TaskContext.h"

View::View(const std::shared_ptr<IIoFacility>& io_facility,
           const std::shared_ptr<IValidator>& validator)
    : validator_(validator), io_facility_(io_facility) {}

void View::SetState(const std::shared_ptr<IStep>& step) {
  current_step_ = step;
}

StepResult View::Run() { return current_step_->Run(); }

void View::ReportInvalidId() {
  std::stringstream ss;
  ss << "Id should be numeric value.\n";
  io_facility_->Print(ss.str()); }

void View::ShowId(TaskId task_id) {
  std::stringstream ss;
  ss << "Id: " << task_id.GetId() << '\n';
  io_facility_->Print(ss.str());
}

void View::ReportNotPresentId(TaskId task_id) {
  std::stringstream ss;
  ss << "Id " << task_id.GetId() << " not present in store.\n";
  io_facility_->Print(ss.str());
}

void View::ReportMultipleId() {
  std::stringstream ss;
  ss << "Multiple Id does not supported for this command.\n";
  io_facility_->Print(ss.str());
}

void View::ReportRequiredId() {
  std::stringstream ss;
  ss << "Id is required for this command.\n";
  io_facility_->Print(ss.str());
}

std::pair<StateEnum, std::vector<TaskId>> View::GetNextCommand() {
  std::stringstream ss;
  ss << "[]: ";
  io_facility_->Print(ss.str());
  std::string result = io_facility_->GetLine();
  return validator_->MakeRequest(result);
}

void View::ShowHelp() {
  std::stringstream ss;
  ss << "Usage:\n"
            << "  add(a) [id]\n"
            << "    Add a new task.\n"
            << "  edit(ed) id\n"
            << "    Edit a task with given id.\n"
            << "  delete(d) id\n"
            << "    Delete a task with the given id.\n"
            << "  complete(c) id\n"
            << "    Complete a task with the given id.\n"
            << "  show(s)\n"
            << "    Show all tasks.\n"
            << "  exit(ex)\n"
            << "    Exit.\n";
  io_facility_->Print(ss.str());
}

void View::ReportUnknownCommand() {
  std::stringstream ss;
  ss
      << "Unknown command. Type 'help' to display all available commands.\n";
  io_facility_->Print(ss.str());
}