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

std::pair<CommandEnum, std::vector<TaskId>> View::GetNextCommand() {
  std::stringstream ss;
  ss << "[]: ";
  io_facility_->Print(ss.str());
  std::string result = io_facility_->GetLine();
  return validator_->MakeRequest(result);
}