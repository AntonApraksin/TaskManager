#ifndef TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_
#define TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_

#include "repl/state_factory/IStateFactory.h"

class DefaultStateFactory : public IStateFactory {
 public:
  DefaultStateFactory(const std::shared_ptr<IIOFacility>& printer,
                      const std::shared_ptr<IValidator>& validator,
                      const std::shared_ptr<TaskManager>& task_manager);

  std::shared_ptr<IREPLState> GetState(StateEnum se) override;
  std::shared_ptr<IREPLSubState> GetREPLState(SubStateEnum sse) override;
  std::shared_ptr<ICommitState> GetCommitState(CommitStateEnum cse) override;

 private:
  std::shared_ptr<MainREPLState> GetMainREPLState();
  std::shared_ptr<AddNewREPLState> GetAddNewREPLState();
  std::shared_ptr<HelpREPLState> GetHelpREPLState();
  std::shared_ptr<UnknownREPLState> GetUnknownREPLState();
  std::shared_ptr<CompleteREPLState> GetCompleteREPLState();
  std::shared_ptr<DeleteREPLState> GetDeleteREPLState();
  std::shared_ptr<EditREPLState> GetEditREPLState();

  std::shared_ptr<ReadTitleREPLSubState> GetReadTitleREPLSubState();
  std::shared_ptr<ReadDateREPLSubState> GetReadDateREPLSubState();
  std::shared_ptr<ReadConfirmationREPLSubState>
  GetReadConfirmationREPLSubState();
  std::shared_ptr<ReadPriorityREPLSubState> GetReadPriorityREPLSubState();
  std::shared_ptr<ReadIdREPLSubState> GetReadIdREPLSubState();

  std::shared_ptr<AddCommitState> GetAddCommitState();
  std::shared_ptr<CompleteCommitState> GetCompleteCommitState();
  std::shared_ptr<DeleteCommitState> GetDeleteCommitState();
  std::shared_ptr<EditCommitState> GetEditCommitState();
  std::shared_ptr<NothingCommitState> GetNothingCommitState();

 private:
  std::shared_ptr<MainREPLState> main_repl_state_;
  std::shared_ptr<AddNewREPLState> add_new_repl_state_;
  std::shared_ptr<HelpREPLState> help_repl_state_;
  std::shared_ptr<UnknownREPLState> unknown_repl_state_;
  std::shared_ptr<CompleteREPLState> complete_repl_state_;
  std::shared_ptr<DeleteREPLState> delete_repl_state_;
  std::shared_ptr<EditREPLState> edit_repl_state_;

  std::shared_ptr<ReadTitleREPLSubState> read_title_repl_sub_state_;
  std::shared_ptr<ReadDateREPLSubState> read_date_repl_sub_state_;
  std::shared_ptr<ReadConfirmationREPLSubState>
      read_confirmation_repl_sub_state_;
  std::shared_ptr<ReadPriorityREPLSubState> read_priority_repl_sub_state_;
  std::shared_ptr<ReadIdREPLSubState> read_id_repl_sub_state_;

  std::shared_ptr<AddCommitState> add_commit_state_;
  std::shared_ptr<CompleteCommitState> complete_commit_state_;
  std::shared_ptr<DeleteCommitState> delete_commit_state_;
  std::shared_ptr<EditCommitState> edit_commit_state_;
  std::shared_ptr<NothingCommitState> nothing_commit_state_;

  std::shared_ptr<IIOFacility> printer_;
  std::shared_ptr<IValidator> validator_;
  std::shared_ptr<TaskManager> task_manager_;
};

#endif  // TASKMANAGER_SRC_REPL_STATE_FACTORY_DEFAULTSTATEFACTORY_H_
