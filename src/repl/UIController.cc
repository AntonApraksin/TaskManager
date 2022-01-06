#include "UIController.h"

#include <filesystem>
#include <fstream>

#include "repl/view/steps/Strings.h"
#include "utils/TaskIdUtils.h"

namespace task_manager {
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

    case CommandEnum::kSave:
      return HandleSave(std::move(args));

    case CommandEnum::kLoad:
      return HandleLoad(std::move(args));

    case CommandEnum::kQuit:
      std::terminate();  // Must be unreachable
  }
}

void UIController::HandleAdd(std::string args) {
  if (args.empty()) {
    return HandleAdd();
  }
  auto token = validator_->ConsumeOneTokenFrom(args);
  auto add_to = validator_->ParseInt(token);
  if (args.empty()) {
    if (add_to) {
      return HandleAdd(CreateTaskId(*add_to));
    } else {
      return ReportMessage(Strings::InvalidId(token));
    }
  }
  return ReportMessage(Strings::kMultipleArgumentDoesNotSupported);
}

void UIController::HandleAdd(TaskId task_id) {
  auto solid_tasks = model_controller_->GetSpecificSolidTasks({task_id});
  if (!solid_tasks) {
    ReportMessage(Strings::NotPresentId(std::to_string(task_id.id())));
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
      ReportMessage(
          Strings::ShowId(std::to_string(result.AccessResult().id())));
    }
  }
}

void UIController::HandleAdd() {
  view_->SetState(step_factory_->GetAddTaskStep());
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    auto result = model_controller_->Add(*task);
    ReportMessage(Strings::ShowId(std::to_string(result.AccessResult().id())));
  }
}

void UIController::HandleEdit(std::string args) {
  if (args.empty()) {
    return ReportMessage(Strings::kRequiredId);
  } else {
    auto token = validator_->ConsumeOneTokenFrom(args);
    auto to_edit = validator_->ParseInt(token);
    if (args.empty()) {
      if (to_edit) {
        return HandleEdit(CreateTaskId(*to_edit));
      } else {
        return ReportMessage(Strings::InvalidId(token));
      }
    }
    return ReportMessage(Strings::kMultipleArgumentDoesNotSupported);
  }
}

void UIController::HandleEdit(TaskId task_id) {
  auto solid_tasks = model_controller_->GetSpecificSolidTasks({task_id});
  if (!solid_tasks) {
    ReportMessage(Strings::NotPresentId(std::to_string(task_id.id())));
    return;
  }
  for (const auto& i : solid_tasks.AccessResult()) {
    if (i.task_id() == task_id) {
      view_->SetState(step_factory_->GetEditTaskStep(i));
    }
  }
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    auto _ = model_controller_->Edit(task_id, *task);
  }
}

void UIController::HandleComplete(std::string args) {
  if (args.empty()) {
    return ReportMessage(Strings::kRequiredId);
  } else {
    std::vector<TaskId> ids;
    std::string token;
    std::optional<int> to_complete;
    for (; !args.empty();) {
      token = validator_->ConsumeOneTokenFrom(args);
      to_complete = validator_->ParseInt(token);
      if (!to_complete) {
        return ReportMessage(Strings::InvalidId(token));
      }
      ids.push_back(CreateTaskId(*to_complete));
    }
    return HandleComplete(std::move(ids));
  }
}

void UIController::HandleComplete(std::vector<TaskId> ids) {
  if (ids.size() > std::set<TaskId>(ids.begin(), ids.end()).size()) {
    ReportMessage(Strings::kRepeatedId);
    return;
  }
  auto solid_tasks = model_controller_->GetSpecificSolidTasks(ids);
  if (!solid_tasks) {
    ReportMessage(Strings::NotPresentId(""));
    return;
  }
  auto has_parent_child_relationship =
      HasParentChildRelationship(solid_tasks.AccessResult(), ids);
  if (has_parent_child_relationship) {
    ReportMessage(Strings::IdIsSubIdOf(
        std::to_string(has_parent_child_relationship->first.id()),
        std::to_string(has_parent_child_relationship->second.id())));
    return;
  }
  view_->SetState(
      step_factory_->GetCompleteTaskStep(solid_tasks.AccessResult()));
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    std::for_each(ids.cbegin(), ids.cend(), [this](auto id) {
      auto _ = model_controller_->Complete(id);
    });
  }
}

void UIController::HandleDelete(std::string args) {
  if (args.empty()) {
    return ReportMessage(Strings::kRequiredId);
  } else {
    std::vector<TaskId> ids;
    std::string token;
    std::optional<int> to_delete;
    for (; !args.empty();) {
      token = validator_->ConsumeOneTokenFrom(args);
      to_delete = validator_->ParseInt(token);
      if (!to_delete) {
        return ReportMessage(Strings::InvalidId(token));
      }
      ids.push_back(CreateTaskId(*to_delete));
    }
    return HandleDelete(std::move(ids));
  }
}
void UIController::HandleDelete(std::vector<TaskId> ids) {
  if (ids.size() > std::set<TaskId>(ids.begin(), ids.end()).size()) {
    ReportMessage(Strings::kRepeatedId);
    return;
  }
  auto solid_tasks = model_controller_->GetSpecificSolidTasks(ids);
  if (!solid_tasks) {
    ReportMessage(Strings::NotPresentId(""));
    return;
  }
  auto has_parent_child_relationship =
      HasParentChildRelationship(solid_tasks.AccessResult(), ids);
  if (has_parent_child_relationship) {
    ReportMessage(Strings::IdIsSubIdOf(
        std::to_string(has_parent_child_relationship->first.id()),
        std::to_string(has_parent_child_relationship->second.id())));
    return;
  }
  view_->SetState(step_factory_->GetDeleteTaskStep(solid_tasks.AccessResult()));
  auto [status, task] = view_->Run();
  if (*status == ConfirmationResult::kYes) {
    std::for_each(ids.cbegin(), ids.cend(),
                  [this](auto id) { auto _ = model_controller_->Delete(id); });
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
        return ReportMessage(Strings::InvalidId(token));
      }
      ids.push_back(CreateTaskId(*to_show));
    }
    return HandleShow(std::move(ids));
  }
}

void UIController::HandleShow(std::vector<TaskId> ids) {
  if (ids.size() > std::set<TaskId>(ids.begin(), ids.end()).size()) {
    ReportMessage(Strings::kRepeatedId);
    return;
  }
  auto solid_tasks = model_controller_->GetSpecificSolidTasks(std::move(ids));

  if (!solid_tasks) {
    ReportMessage(Strings::NotPresentId(""));
    return;
  }
  view_->SetState(step_factory_->GetShowStep(solid_tasks.AccessResult()));
  /*auto [status, task] = */ view_->Run();
}

void UIController::HandleShow() {
  auto solid_tasks = model_controller_->GetAllSolidTasks();
  view_->SetState(step_factory_->GetShowStep(solid_tasks.AccessResult()));
  /*auto [status, task] = */ view_->Run();
}

void UIController::HandleHelp(std::string args) {
  if (args.empty()) {
    return HandleHelp();
  } else {
  }
}

void UIController::HandleHelp() {
  view_->SetState(step_factory_->GetShowHelpStep());
  /*auto [status, task] = */ view_->Run();
}

void UIController::HandleUnknown(std::string /*args*/) {
  view_->SetState(
      step_factory_->GetReportMessageStep(Strings::kUnknownCommand));
  /*auto [status, task] = */ view_->Run();
}

void UIController::ReportMessage(std::string str) {
  view_->SetState(step_factory_->GetReportMessageStep(std::move(str)));
  /*auto [status, task] = */ view_->Run();
}

void UIController::HandleSave(std::string arg) {
  auto filename = validator_->ConsumeOneTokenFrom(arg);
  if (!arg.empty()) {
    ReportMessage(Strings::kMultipleArgumentDoesNotSupported);
    return;
  }
  std::ofstream file(filename);
  if (!file.is_open()) {
    ReportMessage(Strings::ErrorDuringOpeningFile(filename));
    return;
  }
  auto result = model_controller_->SaveTo(file);
  if (result) {
    if (result.GetStatus() == ModelController::Status::kSaveFailure) {
      ReportMessage(Strings::FailureDuringSaving(filename));
      return;
    }
  }
}
void UIController::HandleLoad(std::string arg) {
  auto filename = validator_->ConsumeOneTokenFrom(arg);
  if (!arg.empty()) {
    ReportMessage(Strings::kMultipleArgumentDoesNotSupported);
    return;
  }
  if (!std::filesystem::exists(filename)) {
    ReportMessage(Strings::FilenameDoesNotExist(filename));
    return;
  }
  std::ifstream file(filename);
  if (!file.is_open()) {
    ReportMessage(Strings::ErrorDuringOpeningFile(filename));
    return;
  }
  auto result = model_controller_->LoadFrom(file);
  if (!result) {
    if (result.GetStatus() == ModelController::Status::kLoadFailure) {
      ReportMessage(Strings::FailureDuringLoading(filename));
      return;
    }
  }
}
}  // namespace task_manager
