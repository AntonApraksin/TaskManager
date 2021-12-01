#include "Controller.h"

Controller::Controller(const std::shared_ptr<IIOFacility>& io_facility,
                       const std::shared_ptr<IValidator>& validator,
                       const std::shared_ptr<IStateFactory>& state_factory,
                       const std::shared_ptr<TaskManager>& task_manager)
    : task_manager_(task_manager),
      view_(std::make_unique<View>(io_facility, validator, state_factory)) {}

void Controller::Run() {
  auto command = view_->GetNextCommand();
  for (; command.first != StateEnum::kExit;) {
    PerformAction(command.first, command.second);
    view_->SetState(StateEnum::kMain);
    command = view_->GetNextCommand();
  }
}

void Controller::PerformAction(StateEnum se, const std::vector<TaskId>& ids) {
  switch (se) {
    case StateEnum::kAdd:
      if (ids.empty()) {
        HandleAdd();
      } else if (ids.size() > 1) {
        view_->ReportMultipleId();
      }
      // HandleAdd(ids.at(0)); // TODO
      break;

    case StateEnum::kEdit:
      if (ids.empty()) {
        view_->ReportRequiredId();
      }
      if (ids.size() > 1) {
        view_->ReportMultipleId();
      }
      HandleEdit(ids.at(0));
      break;

    case StateEnum::kComplete:
      if (ids.empty()) {
        view_->ReportRequiredId();
      }
      for (auto i : ids) {
        HandleComplete(i);
      }
      break;

    case StateEnum::kDelete:
      if (ids.empty()) {
        view_->ReportRequiredId();
      }
      for (auto i : ids) {
        HandleDelete(i);
      }
      break;

    case StateEnum::kHelp:
      HandleHelp();
      break;

    case StateEnum::kUnknown:
      view_->ReportUnknownCommand();
      break;

    case StateEnum::kShow:
      HandleShow();  // TODO: add support for subtasks and IDs
      break;

    case StateEnum::kMain:
      throw std::runtime_error("StateEnum::kMain must be unreachable.");
    case StateEnum::kExit:
      throw std::runtime_error("StateEnum::kExit must be unreachable.");
  }
}

void Controller::HandleAdd() {
  view_->SetState(StateEnum::kAdd);
  auto task = view_->GetTask();
  auto confirmation = view_->GetConfirmation();
  if (confirmation == ConfirmationResult::kYes) {
    auto given_id = task_manager_->Add(task);
    view_->ShowId(given_id);
  }
  return;
}

void Controller::HandleEdit(TaskId task_id) {
  view_->SetState(StateEnum::kEdit);
  auto to_edit = task_manager_->Show().find(task_id);
  if (to_edit == task_manager_->Show().end()) {
    // TODO: Report failure
  }
  view_->ShowTask(*to_edit->second);
  auto confirmation = view_->GetConfirmation();
  if (confirmation == ConfirmationResult::kNo) {
    return;
  }
  auto task = view_->GetTask();
  confirmation = view_->GetConfirmation();
  if (confirmation == ConfirmationResult::kYes) {
    task_manager_->Edit(task_id, task);
  }
}

void Controller::HandleComplete(TaskId task_id) {
  view_->SetState(StateEnum::kComplete);
  auto confirmation = view_->GetConfirmation();
  if (confirmation == ConfirmationResult::kYes) {
    task_manager_->Complete(task_id);
  }
}

void Controller::HandleDelete(TaskId task_id) {
  view_->SetState(StateEnum::kDelete);
  auto confirmation = view_->GetConfirmation();
  if (confirmation == ConfirmationResult::kYes) {
    task_manager_->Delete(task_id);
  }
}

void Controller::ShowByMap(const std::map<TaskId, TaskWrapper>& map) {
  for (auto i : map) {
    view_->ShowTaskWithId(i.first, *i.second);
  }
}

void Controller::HandleShow() { ShowByMap(task_manager_->Show()); }

void Controller::HandleHelp() { view_->ShowHelp(); }
