#include "View.h"

#include "repl/substate/TaskContext.h"

View::View(const std::shared_ptr<IIOFacility> &io_facility,
           const std::shared_ptr<IValidator> &validator,
           const std::shared_ptr<IStateFactory> &state_factory)
    : io_facility_(io_facility),
      validator_(validator),
      state_factory_(state_factory) {}

void View::SetState(StateEnum se) {
  switch (se) {
    case StateEnum::kMain:
      io_facility_->ChangePrompt("");
      break;
    case StateEnum::kAdd:
      io_facility_->ChangePrompt("add");
      break;
    case StateEnum::kEdit:
      io_facility_->ChangePrompt("edit");
      break;
    case StateEnum::kHelp:
      break;
    case StateEnum::kComplete:
      break;
    case StateEnum::kDelete:
      break;
    case StateEnum::kExit:
      break;
    case StateEnum::kUnknown:
      break;
    case StateEnum::kShow:
      break;
  }
}

Task View::GetTask() {
  TaskContext sub_context;
  sub_context.PushState(state_factory_->GetREPLState(SubStateEnum::kReadTitle));
  sub_context.PushState(state_factory_->GetREPLState(SubStateEnum::kReadDate));
  sub_context.PushState(
      state_factory_->GetREPLState(SubStateEnum::kReadPriority));
  sub_context.Run();
  return sub_context.GetTaskBuilder().GetTask();
}

void View::ShowTask(Task task) { io_facility_->ShowTask(task); }

void View::ShowId(TaskId task_id) { io_facility_->ShowId(task_id); }

void View::ShowTaskWithId(TaskId task_id, Task task) {
  io_facility_->ShowTaskAndId(task_id, task);
}

void View::ReportInvalidId() { io_facility_->ReportNotValidId(); }

void View::ReportNotPresentId(TaskId task_id) {
  io_facility_->ReportNotPresentId(task_id);
}

void View::ReportMultipleId() { io_facility_->ReportMultipleId(); }

void View::ReportRequiredId() { io_facility_->ReportRequiredId(); }

ConfirmationResult View::GetConfirmation() {
  auto confirm =
      validator_->ParseConfirmation(io_facility_->AskForAConfirmation());
  for (; !confirm;) {
    io_facility_->ReportNotValidConfirmation();
  }
  return *confirm;
}

std::pair<StateEnum, std::vector<TaskId>> View::GetNextCommand() {
  return validator_->MakeRequest(io_facility_->AskForAnAction());
}

void View::ShowHelp() { io_facility_->ShowHelp(); }

void View::ReportUnknownCommand() { io_facility_->ReportUnknownCommand(); }
