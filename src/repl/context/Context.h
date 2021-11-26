#ifndef TASKMANAGER_SRC_REPL_CONTEXT_CONTEXT_H_
#define TASKMANAGER_SRC_REPL_CONTEXT_CONTEXT_H_

#include <memory>

#include "repl/TaskBuilder.h"
#include "repl/commit/ICommitState.h"
#include "repl/print/IPrinter.h"
#include "repl/state/IREPLState.h"
#include "repl/state_factory/IStateFactory.h"
#include "repl/substate/IREPLSubState.h"

class Context {
 public:
  Context(std::unique_ptr<IStateFactory> state_factory);

  void Run();
  void RunInteractor();
  void Commit();

  IStateFactory& GetStateFactory();
  TaskBuilder& GetTaskBuilder();
  TaskId& GetTaskId();

  void SetSubState(const std::shared_ptr<IREPLSubState>&);
  void SetCommitState(const std::shared_ptr<ICommitState>&);

 private:
  TaskBuilder task_builder_;
  TaskId task_id_;

  std::unique_ptr<IStateFactory> state_factory_;

  std::shared_ptr<IREPLState> state_;
  std::shared_ptr<IREPLSubState> sub_state_;
  std::shared_ptr<ICommitState> commit_state_;
};

#endif  // TASKMANAGER_SRC_REPL_CONTEXT_CONTEXT_H_