#include "Controller.h"

Controller::Controller(std::unique_ptr<View> view,
                       std::shared_ptr<TaskManager> task_manager,
                       std::shared_ptr<IValidator> validator,
                       std::unique_ptr<IStepFactory> step_factory)
    : task_manager_(std::move(task_manager)),
      view_(std::move(view)),
      validator_(std::move(validator)),
      step_factory_(std::move(step_factory)) {}

void Controller::Run() {
  auto command = view_->GetNextCommand();
  for (; command.first != CommandEnum::kQuit;) {
    PerformAction(command.first, std::move(command.second));
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

void Controller::PerformAction(CommandEnum se, std::string args) {
  switch (se) {
    case CommandEnum::kAdd:
      return HandleAdd(std::move(args));

    case CommandEnum::kEdit:
      return HandleEdit(std::move(args));

    case CommandEnum::kComplete:
      return HandleComplete(std::move(args));

    case CommandEnum::kDelete:
      return HandleDelete(std::move(args));

    case CommandEnum::kHelp:
      return HandleHelp(std::move(args));

    case CommandEnum::kUnknown:
      return HandleUnknown(std::move(args));

    case CommandEnum::kShow:
      return HandleShow(std::move(args));

    case CommandEnum::kMain:
      std::terminate();  // TODO: Log?
    case CommandEnum::kQuit:
      std::terminate();  // TODO: Log?
  }
}

void Controller::HandleAdd(std::string args) {
  if (args.empty()) {
    return HandleAdd();
  } else {
    auto token = validator_->ConsumeOneTokenFrom(args);
    auto add_to = validator_->ParseInt(token);
    if (args.empty()) {
      if (add_to) {
        return HandleAdd(CreateTaskId(*add_to));
      } else {
        return ReportMessage(MessageEnum::kInvalidId, token);
      }
    }
    return ReportMessage(MessageEnum::kMultipleId);
  }
}

void Controller::HandleAdd(TaskId task_id) {
  auto storage = task_manager_->Show();
  auto add_to = GetTaskById(storage, task_id);
  if (!add_to) {
    ReportMessage(MessageEnum::kNotPresentId, std::to_string(task_id.id()));
    return;
  }
  view_->SetState(step_factory_->GetAddTaskStep(*add_to));
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    auto result = task_manager_->Add(task_id, *task);
    if (result.second.status == TaskManager::ActionResult::Status::kOk) {
      ReportMessage(MessageEnum::kShowId, std::to_string(result.first->id()));
    }
  }
}

void Controller::HandleAdd() {
  view_->SetState(step_factory_->GetAddTaskStep());
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    auto result = task_manager_->Add(*task);  // TODO: handle possible error
    ReportMessage(MessageEnum::kShowId, std::to_string(result.first->id()));
  }
}

void Controller::HandleEdit(std::string args) {
  if (args.empty()) {
    return ReportMessage(MessageEnum::kRequiredId);
  } else {
    auto token = validator_->ConsumeOneTokenFrom(args);
    auto to_edit = validator_->ParseInt(token);
    if (args.empty()) {
      if (to_edit) {
        return HandleEdit(CreateTaskId(*to_edit));
      } else {
        return ReportMessage(MessageEnum::kInvalidId, token);
      }
    }
    return ReportMessage(MessageEnum::kMultipleId);
  }
}

void Controller::HandleEdit(TaskId task_id) {
  auto storage = task_manager_->Show();
  auto to_edit = GetTaskWrapperById(storage, task_id);
  if (!to_edit) {
    return ReportMessage(MessageEnum::kNotPresentId,
                         std::to_string(task_id.id()));
  }
  view_->SetState(step_factory_->GetEditTaskStep(*to_edit));
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    task_manager_->Edit(task_id, *task);  // TODO: handle error
  }
}

void Controller::HandleComplete(std::string args) {
  if (args.empty()) {
    return ReportMessage(MessageEnum::kRequiredId);
  } else {
    std::vector<TaskId> ids;
    std::string token;
    std::optional<int> to_complete;
    for (; !args.empty();) {
      token = validator_->ConsumeOneTokenFrom(args);
      to_complete = validator_->ParseInt(token);
      if (!to_complete) {
        return ReportMessage(MessageEnum::kInvalidId, token);
      }
      ids.push_back(CreateTaskId(*to_complete));
    }
    return HandleComplete(std::move(ids));
  }
}

void Controller::HandleComplete(std::vector<TaskId> ids) {
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
      return ReportMessage(MessageEnum::kNotPresentId,
                           std::to_string(current_id.id()));
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

void Controller::HandleDelete(std::string args) {
  if (args.empty()) {
    return ReportMessage(MessageEnum::kRequiredId);
  } else {
    std::vector<TaskId> ids;
    std::string token;
    std::optional<int> to_delete;
    for (; !args.empty();) {
      token = validator_->ConsumeOneTokenFrom(args);
      to_delete = validator_->ParseInt(token);
      if (!to_delete) {
        return ReportMessage(MessageEnum::kInvalidId, token);
      }
      ids.push_back(CreateTaskId(*to_delete));
    }
    return HandleDelete(std::move(ids));
  }
}
// TODO: Prettify implementation
void Controller::HandleDelete(std::vector<TaskId> ids) {
  if (ids.size() > std::set<TaskId>(ids.begin(), ids.end()).size()) {
    return ReportMessage(MessageEnum::kRepeatedId);
  }
  auto storage = task_manager_->Show();
  ICompleteTaskStep::TaskWrappers tasks;
  TaskId current_id = ids.at(0);
  for (auto id : ids) {
    auto found = storage.Find(id);
    if (found == storage.end()) {
      return ReportMessage(MessageEnum::kNotPresentId,
                           std::to_string(current_id.id()));
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

void Controller::HandleShow(std::string args) {
  if (args.empty()) {
    return HandleShow();
  } else {
    std::vector<TaskId> ids;
    std::string token;
    std::optional<int> to_show;
    for (; !args.empty();) {
      token = validator_->ConsumeOneTokenFrom(args);
      to_show = validator_->ParseInt(token);
      if (!to_show) {
        return ReportMessage(MessageEnum::kInvalidId, token);
      }
      ids.push_back(CreateTaskId(*to_show));
    }
    return HandleShow(std::move(ids));
  }
}

void Controller::HandleShow(std::vector<TaskId> ids) {
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
        ReportMessage(MessageEnum::kNotPresentId,
                      std::to_string(current_id.id()));
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

void Controller::HandleHelp(std::string args) {
  if (args.empty()) {
    return HandleHelp();
  } else {
    // TODO: Add handler here
  }
}

void Controller::HandleHelp() {
  view_->SetState(step_factory_->GetShowHelpStep());
  /*auto [status, task] = */ view_->Run();
}

void Controller::HandleUnknown(std::string args) {
  if (args.empty()) {
    return HandleUnknown();
  } else {
    // TODO: Add handler here
  }
}

void Controller::HandleUnknown() {
  view_->SetState(
      step_factory_->GetReportMessageStep(MessageEnum::kUnknownCommand));
  /*auto [status, task] = */ view_->Run();
}

void Controller::ReportMessage(MessageEnum message_enum) {
  view_->SetState(step_factory_->GetReportMessageStep(message_enum));
  /*auto [status, task] = */ view_->Run();
}

void Controller::ReportMessage(MessageEnum message_enum, std::string arg) {
  view_->SetState(
      step_factory_->GetReportMessageStep(message_enum, std::move(arg)));
  /*auto [status, task] = */ view_->Run();
}