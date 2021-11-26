#ifndef TASKMANAGER_SRC_REPL_COMMIT_ICOMMITSTATE_H_
#define TASKMANAGER_SRC_REPL_COMMIT_ICOMMITSTATE_H_

#include "model/task_manager/TaskManager.h"

class Context;

class ICommitState {
 public:
  virtual ~ICommitState() {}
  ICommitState(const std::shared_ptr<TaskManager>& task_manager)
      : task_manager_(task_manager) {}
  virtual void Execute(Context& ctx) = 0;

 protected:
  std::shared_ptr<TaskManager> task_manager_;
};

class AddCommitState : public ICommitState {
 public:
  using ICommitState::ICommitState;

  void Execute(Context& ctx) override;
};

class EditCommitState : public ICommitState {
 public:
  using ICommitState::ICommitState;

  void Execute(Context& ctx) override;
};

class CompleteCommitState : public ICommitState {
 public:
  using ICommitState::ICommitState;

  void Execute(Context& ctx) override;
};

class DeleteCommitState : public ICommitState {
 public:
  using ICommitState::ICommitState;

  void Execute(Context& ctx) override;
};

class NothingCommitState : public ICommitState {
 public:
  using ICommitState::ICommitState;

  void Execute(Context& ctx) override;
};

#endif  // TASKMANAGER_SRC_REPL_COMMIT_ICOMMITSTATE_H_