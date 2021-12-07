#include "View.h"

#include <iostream>

#include "repl/view/steps/TaskContext.h"

View::View(const std::shared_ptr<IIoFacility>& io_facility,
           const std::shared_ptr<IValidator>& validator)
    : validator_(validator), io_facility_(io_facility) {}

void View::SetState(const std::shared_ptr<IStep>& step) {
  current_step_ = step;
}

StepResult View::Run() { return current_step_->Run(); }

void View::ReportInvalidId() { std::cout << "Id should be numeric value.\n"; }

void View::ShowId(TaskId task_id) {
  std::cout << "Id: " << task_id.GetId() << '\n';
}

void View::ReportNotPresentId(TaskId task_id) {
  std::cout << "Id " << task_id.GetId() << " not present in store.\n";
}

void View::ReportMultipleId() {
  std::cout << "Multiple Id does not supported for this command.\n";
}

void View::ReportRequiredId() {
  std::cout << "Id is required for this command.\n";
}

std::pair<StateEnum, std::vector<TaskId>> View::GetNextCommand() {
  std::cout << "[]: ";
  std::string result;
  std::getline(std::cin, result);
  return validator_->MakeRequest(result);
}

void View::ShowHelp() {
  std::cout << "Usage:\n"
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
}

void View::ReportUnknownCommand() {
  std::cout
      << "Unknown command. Type 'help' to display all available commands.\n";
}