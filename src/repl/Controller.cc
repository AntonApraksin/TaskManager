#include "Controller.h"

// TODO: handle errors with id that is not present

Controller::Controller(const std::shared_ptr<IValidator>& validator,
                       const std::shared_ptr<TaskManager>& task_manager,
                       std::unique_ptr<IStepFactory> step_factory)
    : task_manager_(task_manager),
      view_(std::make_unique<View>(validator)),
      step_factory_(std::move(step_factory)) {}

void Controller::Run() {
  auto command = view_->GetNextCommand();
  for (; command.first != StateEnum::kExit;) {
    PerformAction(command.first, command.second);
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
      } else {
        HandleAdd(ids.at(0));
      }
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

void Controller::HandleAdd(TaskId task_id) {
  auto add_to = *task_manager_->Show().Find(task_id);
  view_->SetState(step_factory_->GetAddTaskREPLState(add_to));
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    auto given_id = task_manager_->Add(task_id, *task);
    view_->ShowId(given_id);
  }
}

void Controller::HandleAdd() {
  view_->SetState(step_factory_->GetAddTaskREPLState());
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    auto given_id = task_manager_->Add(*task);
    view_->ShowId(given_id);
  }
}

void Controller::HandleEdit(TaskId task_id) {
  auto to_edit = task_manager_->Show().Find(task_id);
  view_->SetState(step_factory_->GetEditTaskREPLState(to_edit));
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    task_manager_->Edit(task_id, *task);
  }
}

void Controller::HandleComplete(TaskId task_id) {  // TODO: Make it vector
  auto to_complete = task_manager_->Show().Find(task_id);
  view_->SetState(step_factory_->GetCompleteTaskREPLState({to_complete}));
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    task_manager_->Complete(task_id);
  }
}

void Controller::HandleDelete(TaskId task_id) {  // TODO: Make it vector
  auto to_delete = task_manager_->Show().Find(task_id);
  view_->SetState(step_factory_->GetDeleteTaskREPLState({to_delete}));
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    task_manager_->Delete(task_id);
  }
}

void Controller::HandleShow() {
  view_->SetState(
      step_factory_->GetShowAllTasksREPLState(task_manager_->Show()));
  view_->Run();
}

void Controller::HandleHelp() { view_->ShowHelp(); }
