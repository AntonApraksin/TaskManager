#include "DefaultStateFactory.h"

#define GENERATE_GETTER_FOR_POINTER(cls, name, args) \
  if (name == nullptr) {                             \
    name = std::make_shared<cls> args;               \
  }                                                  \
  return name;

DefaultStateFactory::DefaultStateFactory(
    const std::shared_ptr<IPrinter>& printer,
    const std::shared_ptr<IValidator>& validator,
    const std::shared_ptr<TaskManager>& task_manager)
    : printer_(printer), validator_(validator), task_manager_(task_manager) {}

std::shared_ptr<IREPLState> DefaultStateFactory::GetState(StateEnum se) {
  switch (se) {
    case StateEnum::kDefault:
      return GetMainREPLState();
    case StateEnum::kMain:
      return GetMainREPLState();
    case StateEnum::kAdd:
      return GetAddNewREPLState();
    case StateEnum::kHelp:
      return GetHelpREPLState();
    case StateEnum::kComplete:
      return GetCompleteREPLState();
    case StateEnum::kDelete:
      return GetDeleteREPLState();
    case StateEnum::kEdit:
      return GetEditREPLState();
    case StateEnum::kExit:
      return {};

    case StateEnum::kUnknown:
      return GetUnknownREPLState();
  }
}

std::shared_ptr<IREPLSubState> DefaultStateFactory::GetSubState(
    SubStateEnum sse) {
  switch (sse) {
    case SubStateEnum::kReadTitle:
      return GetReadTitleREPLSubState();
    case SubStateEnum::kReadConfirmation:
      return GetReadConfirmationREPLSubState();
    case SubStateEnum::kReadDate:
      return GetReadDateREPLSubState();
    case SubStateEnum::kReadId:
      return GetReadIdREPLSubState();
    case SubStateEnum::kReadPriority:
      return GetReadPriorityREPLSubState();
  }
}

std::shared_ptr<ICommitState> DefaultStateFactory::GetCommitState(
    CommitStateEnum cse) {
  switch (cse) {
    case CommitStateEnum::kAdd:
      return GetAddCommitState();
    case CommitStateEnum::kEdit:
      return GetEditCommitState();
    case CommitStateEnum::kComplete:
      return GetCompleteCommitState();
    case CommitStateEnum::kDelete:
      return GetDeleteCommitState();
    case CommitStateEnum::kNothing:
      return GetNothingCommitState();
  }
}

std::shared_ptr<MainREPLState> DefaultStateFactory::GetMainREPLState() {
  GENERATE_GETTER_FOR_POINTER(MainREPLState, main_repl_state_,
                              (printer_, validator_));
}

std::shared_ptr<AddNewREPLState> DefaultStateFactory::GetAddNewREPLState() {
  GENERATE_GETTER_FOR_POINTER(AddNewREPLState, add_new_repl_state_,
                              (printer_, validator_));
}

std::shared_ptr<HelpREPLState> DefaultStateFactory::GetHelpREPLState() {
  GENERATE_GETTER_FOR_POINTER(HelpREPLState, help_repl_state_, (printer_));
}
std::shared_ptr<UnknownREPLState> DefaultStateFactory::GetUnknownREPLState() {
  GENERATE_GETTER_FOR_POINTER(UnknownREPLState, unknown_repl_state_,
                              (printer_));
}

std::shared_ptr<CompleteREPLState> DefaultStateFactory::GetCompleteREPLState() {
  GENERATE_GETTER_FOR_POINTER(CompleteREPLState, complete_repl_state_,
                              (printer_, validator_));
}

std::shared_ptr<DeleteREPLState> DefaultStateFactory::GetDeleteREPLState() {
  GENERATE_GETTER_FOR_POINTER(DeleteREPLState, delete_repl_state_,
                              (printer_, validator_));
}

std::shared_ptr<EditREPLState> DefaultStateFactory::GetEditREPLState() {
  GENERATE_GETTER_FOR_POINTER(EditREPLState, edit_repl_state_,
                              (printer_, validator_));
}

std::shared_ptr<ReadTitleREPLSubState>
DefaultStateFactory::GetReadTitleREPLSubState() {
  GENERATE_GETTER_FOR_POINTER(ReadTitleREPLSubState, read_title_repl_sub_state_,
                              (printer_, validator_));
}

std::shared_ptr<ReadDateREPLSubState>
DefaultStateFactory::GetReadDateREPLSubState() {
  GENERATE_GETTER_FOR_POINTER(ReadDateREPLSubState, read_date_repl_sub_state_,
                              (printer_, validator_));
}

std::shared_ptr<ReadConfirmationREPLSubState>
DefaultStateFactory::GetReadConfirmationREPLSubState() {
  GENERATE_GETTER_FOR_POINTER(ReadConfirmationREPLSubState,
                              read_confirmation_repl_sub_state_,
                              (printer_, validator_));
}
std::shared_ptr<ReadPriorityREPLSubState>
DefaultStateFactory::GetReadPriorityREPLSubState() {
  GENERATE_GETTER_FOR_POINTER(ReadPriorityREPLSubState,
                              read_priority_repl_sub_state_,
                              (printer_, validator_));
}
std::shared_ptr<ReadIdREPLSubState>
DefaultStateFactory::GetReadIdREPLSubState() {
  GENERATE_GETTER_FOR_POINTER(ReadIdREPLSubState, read_id_repl_sub_state_,
                              (printer_, validator_));
}

std::shared_ptr<AddCommitState> DefaultStateFactory::GetAddCommitState() {
  GENERATE_GETTER_FOR_POINTER(AddCommitState, add_commit_state_,
                              (task_manager_));
}

std::shared_ptr<CompleteCommitState>
DefaultStateFactory::GetCompleteCommitState() {
  GENERATE_GETTER_FOR_POINTER(CompleteCommitState, complete_commit_state_,
                              (task_manager_));
}

std::shared_ptr<DeleteCommitState> DefaultStateFactory::GetDeleteCommitState() {
  GENERATE_GETTER_FOR_POINTER(DeleteCommitState, delete_commit_state_,
                              (task_manager_));
}

std::shared_ptr<EditCommitState> DefaultStateFactory::GetEditCommitState() {
  GENERATE_GETTER_FOR_POINTER(EditCommitState, edit_commit_state_,
                              (task_manager_));
}

std::shared_ptr<NothingCommitState>
DefaultStateFactory::GetNothingCommitState() {
  GENERATE_GETTER_FOR_POINTER(NothingCommitState, nothing_commit_state_,
                              (task_manager_));
}