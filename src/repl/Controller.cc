#include "Controller.h"

Controller::Controller(std::unique_ptr<View> view,
                       const std::shared_ptr<TaskManager>& task_manager,
                       std::unique_ptr<IStepFactory> step_factory)
    : task_manager_(task_manager),
      view_(std::move(view)),
      step_factory_(std::move(step_factory)) {}

void Controller::Run() {
  auto command = view_->GetNextCommand();
  for (; command.first != CommandEnum::kQuit;) {
    if (command.second) {
      PerformAction(command.first, *command.second);
    } else {
      ReportMessage(MessageEnum::kInvalidId);
    }
    command = view_->GetNextCommand();
  }
}
std::optional<Task> Controller::GetTaskById(const TaskStorage& storage,
                                            TaskId id) {
  auto found = storage.Find(id);
  if (found == storage.cend()) {
    return {};
  }
  return *found->second;
}

std::optional<TaskWrapper> Controller::GetTaskWrapperById(
    const TaskStorage& storage, TaskId id) {
  auto found = storage.Find(id);
  if (found == storage.cend()) {
    return {};
  }
  return found->second;
}

void Controller::PerformAction(CommandEnum se, const std::vector<TaskId>& ids) {
  switch (se) {
    case CommandEnum::kAdd:
      if (ids.empty()) {
        HandleAdd();
      } else if (ids.size() > 1) {
        ReportMessage(MessageEnum::kMultipleId);
      } else {
        HandleAdd(ids.at(0));
      }
      break;

    case CommandEnum::kEdit:
      if (ids.empty()) {
        ReportMessage(MessageEnum::kRequiredId);
      } else if (ids.size() > 1) {
        ReportMessage(MessageEnum::kMultipleId);
      } else {
        HandleEdit(ids.at(0));
      }
      break;

    case CommandEnum::kComplete:
      if (ids.empty()) {
        ReportMessage(MessageEnum::kRequiredId);
      } else {
        HandleComplete(ids);
      }
      break;

    case CommandEnum::kDelete:
      if (ids.empty()) {
        ReportMessage(MessageEnum::kRequiredId);
      } else {
        HandleDelete(ids);
      }
      break;

    case CommandEnum::kHelp:
      HandleHelp();
      break;

    case CommandEnum::kUnknown:
      ReportMessage(MessageEnum::kUnknownCommand);
      break;

    case CommandEnum::kShow:
      if (ids.empty()) {
        HandleShow();
      } else {
        HandleShow(ids);
      }
      break;

    case CommandEnum::kMain:
      throw std::runtime_error("CommandEnum::kMain must be unreachable.");
    case CommandEnum::kQuit:
      throw std::runtime_error("CommandEnum::kQuit must be unreachable.");
  }
}

void Controller::HandleAdd(TaskId task_id) {
  auto storage = task_manager_->Show();
  auto add_to = GetTaskById(storage, task_id);
  if (!add_to) {
    ReportMessage(MessageEnum::kNotPresentId, task_id);
    return;
  }
  view_->SetState(step_factory_->GetAddTaskStep(*add_to));
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    auto result = task_manager_->Add(task_id, *task);
    if (result.second.status == TaskManager::ActionResult::Status::kOk) {
      ReportMessage(MessageEnum::kShowId, *result.first);
    }
  }
}

void Controller::HandleAdd() {
  view_->SetState(step_factory_->GetAddTaskStep());
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    auto result = task_manager_->Add(*task);  // TODO: handle possible error
    ReportMessage(MessageEnum::kShowId, *result.first);
  }
}

void Controller::HandleEdit(TaskId task_id) {
  auto storage = task_manager_->Show();
  auto to_edit = GetTaskWrapperById(storage, task_id);
  if (!to_edit) {
    ReportMessage(MessageEnum::kNotPresentId, task_id);
    return;
  }
  view_->SetState(step_factory_->GetEditTaskStep(*to_edit));
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    task_manager_->Edit(task_id, *task);  // TODO: handle error
  }
}

// TODO: Prettify implementation
void Controller::HandleComplete(const std::vector<TaskId>& ids) {
  if (ids.size() > std::set<TaskId>(ids.begin(), ids.end()).size()) {
    ReportMessage(MessageEnum::kRepeatedId);
    return;
  }
  auto storage = task_manager_->Show();

  ICompleteTaskStep::TaskWrappers tasks;

  TaskId current_id = ids.at(0);
  for (auto id : ids) {
    auto found = storage.Find(id);
    if (found == storage.end()) {
      ReportMessage(MessageEnum::kNotPresentId, current_id);
      return;
    }
    tasks.push_back(storage.Find(id)->second);
  }

  view_->SetState(step_factory_->GetCompleteTaskStep(std::move(tasks)));
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    std::for_each(ids.cbegin(), ids.cend(),
                  [this](auto id) { task_manager_->Complete(id); });
  }
}

// TODO: Prettify implementation
void Controller::HandleDelete(const std::vector<TaskId>& ids) {
  if (ids.size() > std::set<TaskId>(ids.begin(), ids.end()).size()) {
    ReportMessage(MessageEnum::kRepeatedId);
    return;
  }
  auto storage = task_manager_->Show();
  ICompleteTaskStep::TaskWrappers tasks;
  TaskId current_id = ids.at(0);
  for (auto id : ids) {
    auto found = storage.Find(id);
    if (found == storage.end()) {
      ReportMessage(MessageEnum::kNotPresentId, current_id);
      return;
    }
    tasks.push_back(storage.Find(id)->second);
  }
  view_->SetState(step_factory_->GetDeleteTaskStep(tasks));
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    std::for_each(ids.cbegin(), ids.cend(),
                  [this](auto id) { task_manager_->Delete(id); });
  }
}

void Controller::HandleShow(const std::vector<TaskId>& ids) {
  auto storage = task_manager_->Show();
  IShowNTasksStep::TaskWrappers tasks;
  std::vector<TaskId> seen;
  TaskId current_id = ids.at(0);
  for (auto id : ids) {
    if (std::find(seen.begin(), seen.end(), id) == seen.end()) {
      seen.push_back(id);
      current_id = id;
      auto found = storage.Find(id);
      if (found == storage.end()) {
        ReportMessage(MessageEnum::kNotPresentId, current_id);
        return;
      }
      tasks.push_back(storage.Find(id)->second);
    }
  }
  view_->SetState(step_factory_->GetShowNTasksStep(tasks));
  /*auto [status, task] = */ view_->Run();
}

void Controller::HandleShow() {
  auto to_show = task_manager_->Show();
  view_->SetState(step_factory_->GetShowAllTasksStep(to_show));
  /*auto [status, task] = */ view_->Run();
}

void Controller::HandleHelp() {
  view_->SetState(step_factory_->GetShowHelpStep());
  /*auto [status, task] = */ view_->Run();
}

void Controller::ReportMessage(MessageEnum message_enum) {
  view_->SetState(step_factory_->GetReportMessageStep(message_enum));
  /*auto [status, task] = */ view_->Run();
}

void Controller::ReportMessage(MessageEnum message_enum, TaskId task_id) {
  view_->SetState(step_factory_->GetReportMessageStep(message_enum, task_id));
  /*auto [status, task] = */ view_->Run();
}