#include "Controller.h"

// TODO: inject View

Controller::Controller(std::unique_ptr<View> view,
                       const std::shared_ptr<TaskManager>& task_manager,
                       std::unique_ptr<IStepFactory> step_factory)
    : task_manager_(task_manager),
      view_(std::move(view)),
      step_factory_(std::move(step_factory)) {}

void Controller::Run() {
  auto command = view_->GetNextCommand();
  for (; command.first != CommandEnum::kQuit;) {
    PerformAction(command.first, command.second);
    command = view_->GetNextCommand();
  }
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
      HandleShow();  // TODO: add support for subtasks and IDs
      break;

    case CommandEnum::kMain:
      throw std::runtime_error("CommandEnum::kMain must be unreachable.");
    case CommandEnum::kQuit:
      throw std::runtime_error("CommandEnum::kQuit must be unreachable.");
  }
}

void Controller::HandleAdd(TaskId task_id) {
  try {
    auto add_to = *task_manager_->Show().Find(task_id);
    view_->SetState(step_factory_->GetAddTaskStep(add_to));
    auto [status, task] = view_->Run();
    if (*status == ConfirmationResult::kYes) {
      auto given_id = task_manager_->Add(task_id, *task);
      ReportMessage(MessageEnum::kShowId, given_id);
    }
  } catch (const std::runtime_error&) {
    ReportMessage(MessageEnum::kNotPresentId, task_id);
  }
}

void Controller::HandleAdd() {
  view_->SetState(step_factory_->GetAddTaskStep());
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    auto given_id = task_manager_->Add(*task);
    ReportMessage(MessageEnum::kShowId, given_id);
  }
}

void Controller::HandleEdit(TaskId task_id) {
  try {
    auto to_edit = task_manager_->Show().Find(task_id);
    view_->SetState(step_factory_->GetEditTaskStep(to_edit));
    auto [status, task] = view_->Run();
    if (*status == ConfirmationResult::kYes) {
      task_manager_->Edit(task_id, *task);
    }
  } catch (const std::runtime_error&) {
    ReportMessage(MessageEnum::kNotPresentId, task_id);
  }
}

// TODO: Prettify implementation
void Controller::HandleComplete(const std::vector<TaskId>& ids) {
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
    ReportMessage(MessageEnum::kNotPresentId, current_id);
  }

  view_->SetState(step_factory_->GetCompleteTaskStep(std::move(tasks)));
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
void Controller::HandleDelete(const std::vector<TaskId>& ids) {
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
    ReportMessage(MessageEnum::kNotPresentId, current_id);
  }
  view_->SetState(step_factory_->GetDeleteTaskStep(tasks));
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
  view_->SetState(step_factory_->GetShowAllTasksStep(to_show));
  view_->Run();
}

void Controller::HandleHelp() {
  view_->SetState(step_factory_->GetShowHelpStep());
  view_->Run();
}

void Controller::ReportMessage(MessageEnum message_enum) {
  view_->SetState(step_factory_->GetReportMessageStep(message_enum));
  view_->Run();
}

void Controller::ReportMessage(MessageEnum message_enum, TaskId task_id) {
  view_->SetState(step_factory_->GetReportMessageStep(message_enum, task_id));
  view_->Run();
}