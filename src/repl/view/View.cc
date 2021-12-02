#include "View.h"

#include "repl/substate/TaskContext.h"

#include <iostream>

View::View(const std::shared_ptr<IIOFacility> &io_facility,
           const std::shared_ptr<IValidator> &validator)
    : io_facility_(io_facility),
      validator_(validator){}

void View::SetState(const std::shared_ptr<IStep>& step) {
  current_step_ = step;
}

StepResult View::Run() {
  return current_step_->Run();
}

void View::ReportInvalidId() { io_facility_->ReportNotValidId(); }

void View::ReportNotPresentId(TaskId task_id) {
  io_facility_->ReportNotPresentId(task_id);
}

void View::ReportMultipleId() { io_facility_->ReportMultipleId(); }

void View::ReportRequiredId() { io_facility_->ReportRequiredId(); }

std::pair<StateEnum, std::vector<TaskId>> View::GetNextCommand() {
  return validator_->MakeRequest(io_facility_->AskForAnAction());
}

void View::ShowHelp() {
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
}

void View::ReportUnknownCommand() {
  std::cout <<
            "Unknown command. Type 'help' to display all available commands.";
}
