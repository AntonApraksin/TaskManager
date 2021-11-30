#ifndef TASKMANAGER_SRC_REPL_COMMIT_ICOMMITSTATE_H_
#define TASKMANAGER_SRC_REPL_COMMIT_ICOMMITSTATE_H_

#include "model/task_manager/TaskManager.h"
#include "repl/substate/ISubState.h"

class Context;

class ICommitState : public ISubState {
 public:
  ICommitState(const std::shared_ptr<TaskManager>& task_manager)
      : task_manager_(task_manager) {}

 protected:
  std::shared_ptr<TaskManager> task_manager_;
};

class AddCommitState : public ICommitState {
 public:
  using ICommitState::ICommitState;

  void Execute(TaskContext& ctx) override;
};

class EditCommitState : public ICommitState {
 public:
  using ICommitState::ICommitState;

  void Execute(TaskContext& ctx) override;
};

class CompleteCommitState : public ICommitState {
 public:
  using ICommitState::ICommitState;

  void Execute(TaskContext& ctx) override;
};

class DeleteCommitState : public ICommitState {
 public:
  using ICommitState::ICommitState;

  void Execute(TaskContext& ctx) override;
};

class NothingCommitState : public ICommitState {
 public:
  using ICommitState::ICommitState;

  void Execute(TaskContext& ctx) override;
};

#endif  // TASKMANAGER_SRC_REPL_COMMIT_ICOMMITSTATE_H_