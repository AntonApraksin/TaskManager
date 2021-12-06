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
      HandleComplete(ids);
      break;

    case StateEnum::kDelete:
      if (ids.empty()) {
        view_->ReportRequiredId();
      }
      HandleDelete(ids);
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
  try {
    auto add_to = *task_manager_->Show().Find(task_id);
    view_->SetState(step_factory_->GetAddTaskREPLState(add_to));
    auto [status, task] = view_->Run();
    if (*status == ConfirmationResult::kYes) {
      auto given_id = task_manager_->Add(task_id, *task);
      view_->ShowId(given_id);
    }
  } catch (const std::runtime_error&) {
    view_->ReportNotPresentId(task_id);
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
  try {
    auto to_edit = task_manager_->Show().Find(task_id);
    view_->SetState(step_factory_->GetEditTaskREPLState(to_edit));
    auto [status, task] = view_->Run();
    if (*status == ConfirmationResult::kYes) {
      task_manager_->Edit(task_id, *task);
    }
  } catch (const std::runtime_error&) {
    view_->ReportNotPresentId(task_id);
  }
}

// TODO: Prettify implementation
void Controller::HandleComplete(
    const std::vector<TaskId>& ids) {
  ICompleteTaskStep::TaskWrappers tasks;

  TaskId current_id = ids.at(0);
  std::vector<TaskId> seen;
  auto storage = task_manager_->Show();
  try {
    std::for_each(ids.cbegin(), ids.cend(), [&](auto id) {
      if (std::find(seen.begin(), seen.end(), id) == seen.end()) {
        seen.push_back(id);
        current_id = id;
        tasks.push_back(storage.Find(id));
      }
    });
  } catch (const std::runtime_error&) {
    view_->ReportNotPresentId(current_id);
  }

  view_->SetState(step_factory_->GetCompleteTaskREPLState(std::move(tasks)));
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    std::for_each(seen.cbegin(), seen.cend(), [this](auto id) {
      try {
        task_manager_->Complete(id);
      } catch (const std::runtime_error&) {
      }
    });
  }
}

// TODO: Prettify implementation
void Controller::HandleDelete(
    const std::vector<TaskId>& ids) {
  ICompleteTaskStep::TaskWrappers tasks;
  std::vector<TaskId> seen;
  TaskId current_id = ids.at(0);
  auto storage = task_manager_->Show();
  try {
    std::for_each(ids.cbegin(), ids.cend(), [&](auto id) {
      if (std::find(seen.begin(), seen.end(), id) == seen.end()) {
        seen.push_back(id);
        current_id = id;
        tasks.push_back(storage.Find(id));
      }
    });
  } catch (const std::runtime_error&) {
    view_->ReportNotPresentId(current_id);
  }
  view_->SetState(step_factory_->GetDeleteTaskREPLState(tasks));
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    std::for_each(seen.cbegin(), seen.cend(), [this](auto id) {
      try {
        task_manager_->Delete(id);
      } catch (const std::runtime_error&) {
      }
    });
  }
}

void Controller::HandleShow() {
  auto to_show = task_manager_->Show();
  view_->SetState(step_factory_->GetShowAllTasksREPLState(to_show));
  view_->Run();
}

void Controller::HandleHelp() { view_->ShowHelp(); }
