#include "View.h"

#include "repl/view/steps/Strings.h"
#include "repl/view/steps/TaskContext.h"

View::View(const std::shared_ptr<IIoFacility>& io_facility,
           const std::shared_ptr<IValidator>& validator)
    : validator_(validator), io_facility_(io_facility) {}

void View::SetState(const std::shared_ptr<IStep>& step) {
  current_step_ = step;
}

StepResult View::Run() { return current_step_->Run(); }

std::pair<CommandEnum, std::string> View::GetNextCommand() {
  std::string result = PrintAndGet(*io_facility_, Strings::GetPrompt(""));
  return validator_->MakeRequest(result);
}