#include "UIController.h"

UIController::UIController(std::unique_ptr<View> view,
                           std::shared_ptr<ModelController> model_controller,
                           std::shared_ptr<IValidator> validator,
                           std::unique_ptr<IStepFactory> step_factory)
    : model_controller_(std::move(model_controller)),
      view_(std::move(view)),
      validator_(std::move(validator)),
      step_factory_(std::move(step_factory)) {}

void UIController::Run() {
  auto command = view_->GetNextCommand();
  for (; command.first != CommandEnum::kQuit;) {
    PerformAction(command.first, std::move(command.second));
    command = view_->GetNextCommand();
  }
}

void UIController::PerformAction(CommandEnum se, std::string args) {
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

void UIController::HandleAdd(std::string args) {
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

void UIController::HandleAdd(TaskId task_id) {
  auto solid_tasks = model_controller_->GetSpecificSolidTasks({task_id});
  if (!solid_tasks) {
    ReportMessage(MessageEnum::kNotPresentId, std::to_string(task_id.id()));
    return;
  }
  for (const auto& i : solid_tasks.AccessResult()) {
    if (i.task_id() == task_id) {
      view_->SetState(step_factory_->GetAddTaskStep(i));
    }
  }
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    auto result = model_controller_->Add(task_id, *task);
    if (result) {
      ReportMessage(MessageEnum::kShowId,
                    std::to_string(result.AccessResult().id()));
    }
  }
}

void UIController::HandleAdd() {
  view_->SetState(step_factory_->GetAddTaskStep());
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    auto result = model_controller_->Add(*task);  // TODO: handle possible error
    ReportMessage(MessageEnum::kShowId,
                  std::to_string(result.AccessResult().id()));
  }
}

void UIController::HandleEdit(std::string args) {
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

void UIController::HandleEdit(TaskId task_id) {
  auto solid_tasks = model_controller_->GetSpecificSolidTasks({task_id});
  if (!solid_tasks) {
    ReportMessage(MessageEnum::kNotPresentId, std::to_string(task_id.id()));
    return;
  }
  for (const auto& i : solid_tasks.AccessResult()) {
    if (i.task_id() == task_id) {
      view_->SetState(step_factory_->GetEditTaskStep(i));
    }
  }
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    model_controller_->Edit(task_id, *task);  // TODO: handle error
  }
}

void UIController::HandleComplete(std::string args) {
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

std::optional<std::pair<TaskId, TaskId>> HasParentChildRelationship(
    const SolidTasks& tasks, const std::vector<TaskId>& ids) {
  std::unordered_map<TaskId, std::vector<TaskId>> visited;
  TaskId current_root_id;
  for (const auto& i : tasks) {
    if (i.has_parent_id()) {
      visited[current_root_id].push_back(i.task_id());
    } else {
      current_root_id = i.task_id();
    }
  }
  for (const auto& i : ids) {
    auto it = std::find_if(visited.begin(), visited.end(), [&i](auto it) {
      return std::find(it.second.begin(), it.second.end(), i) !=
             it.second.end();
    });
    if (it != visited.end()) {
      return std::make_pair(it->first, i);
    }
  }
  return {};
}

void UIController::HandleComplete(std::vector<TaskId> ids) {
  if (ids.size() > std::set<TaskId>(ids.begin(), ids.end()).size()) {
    ReportMessage(MessageEnum::kRepeatedId);
    return;
  }
  auto solid_tasks = model_controller_->GetSpecificSolidTasks(ids);
  if (!solid_tasks) {
    ReportMessage(MessageEnum::kNotPresentId);
    return;
  }
  auto has_parent_child_relationship =
      HasParentChildRelationship(solid_tasks.AccessResult(), ids);
  if (has_parent_child_relationship) {
    // TODO: Handle Error
    return;
  }
  view_->SetState(
      step_factory_->GetCompleteTaskStep(solid_tasks.AccessResult()));
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    std::for_each(ids.cbegin(), ids.cend(),
                  [this](auto id) { model_controller_->Complete(id); });
  }
}

void UIController::HandleDelete(std::string args) {
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
void UIController::HandleDelete(std::vector<TaskId> ids) {
  if (ids.size() > std::set<TaskId>(ids.begin(), ids.end()).size()) {
    ReportMessage(MessageEnum::kRepeatedId);
    return;
  }
  auto solid_tasks = model_controller_->GetSpecificSolidTasks(ids);
  if (!solid_tasks) {
    ReportMessage(MessageEnum::kNotPresentId);
    return;
  }
  auto has_parent_child_relationship =
      HasParentChildRelationship(solid_tasks.AccessResult(), ids);
  if (has_parent_child_relationship) {
    // TODO: Handle Error
    return;
  }
  view_->SetState(step_factory_->GetDeleteTaskStep(solid_tasks.AccessResult()));
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    std::for_each(ids.cbegin(), ids.cend(),
                  [this](auto id) { model_controller_->Delete(id); });
  }
}

void UIController::HandleShow(std::string args) {
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

void UIController::HandleShow(std::vector<TaskId> ids) {
  if (ids.size() > std::set<TaskId>(ids.begin(), ids.end()).size()) {
    ReportMessage(MessageEnum::kRepeatedId);
    return;
  }
  auto solid_tasks = model_controller_->GetSpecificSolidTasks(std::move(ids));

  if (!solid_tasks) {
    ReportMessage(MessageEnum::kNotPresentId);
    return;
  }
  view_->SetState(step_factory_->GetShowStep(solid_tasks.AccessResult()));
  /*auto [status, task] = */ view_->Run();
}

void UIController::HandleShow() {
  auto solid_tasks = model_controller_->GetAllSolidTasks();
  if (!solid_tasks) {
    ReportMessage(MessageEnum::kNotPresentId);
    return;
  }
  view_->SetState(step_factory_->GetShowStep(solid_tasks.AccessResult()));
  /*auto [status, task] = */ view_->Run();
}

void UIController::HandleHelp(std::string args) {
  if (args.empty()) {
    return HandleHelp();
  } else {
    // TODO: Add handler here
  }
}

void UIController::HandleHelp() {
  view_->SetState(step_factory_->GetShowHelpStep());
  /*auto [status, task] = */ view_->Run();
}

void UIController::HandleUnknown(std::string args) {
  if (args.empty()) {
    return HandleUnknown();
  } else {
    // TODO: Add handler here
  }
}

void UIController::HandleUnknown() {
  view_->SetState(
      step_factory_->GetReportMessageStep(MessageEnum::kUnknownCommand));
  /*auto [status, task] = */ view_->Run();
}

void UIController::ReportMessage(MessageEnum message_enum) {
  view_->SetState(step_factory_->GetReportMessageStep(message_enum));
  /*auto [status, task] = */ view_->Run();
}

void UIController::ReportMessage(MessageEnum message_enum, std::string arg) {
  view_->SetState(
      step_factory_->GetReportMessageStep(message_enum, std::move(arg)));
  /*auto [status, task] = */ view_->Run();
}