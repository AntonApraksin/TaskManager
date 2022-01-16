#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_COMMANDS_COMMANDS_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_COMMANDS_COMMANDS_H_

#include <istream>
#include <memory>
#include <ostream>

#include "interactor/state_machine/Context.h"
#include "persistence/FilePersistence.h"

namespace task_manager {
class Command {
 public:
  virtual Context execute(ModelController&) = 0;

  virtual ~Command(){};
};

class AddTaskCommand : public Command {
 public:
  explicit AddTaskCommand(Task);
  Context execute(ModelController&) override;

 private:
  Task task_;
};

class AddSubtaskCommand : public Command {
 public:
  AddSubtaskCommand(TaskId, Task);
  Context execute(ModelController&) override;

 private:
  TaskId task_id_;
  Task task_;
};

class EditTaskCommand : public Command {
 public:
  EditTaskCommand(TaskId, Task);

  Context execute(ModelController&) override;

 private:
  TaskId task_id_;
  Task task_;
};

class CompleteTasksCommand : public Command {
 public:
  explicit CompleteTasksCommand(std::vector<TaskId>);

  Context execute(ModelController&) override;

 private:
  std::vector<TaskId> task_ids_;
};

class DeleteTasksCommand : public Command {
 public:
  explicit DeleteTasksCommand(std::vector<TaskId>);

  Context execute(ModelController&) override;

 private:
  std::vector<TaskId> task_ids_;
};

class GetSpecifiedTasksCommand : public Command {
 public:
  explicit GetSpecifiedTasksCommand(std::vector<TaskId>);

  Context execute(ModelController&) override;

 private:
  std::vector<TaskId> task_ids_;
};

class GetAllTasksCommand : public Command {
 public:
  Context execute(ModelController&) override;
};

class LoadTasksFromFileCommand : public Command {
 public:
  explicit LoadTasksFromFileCommand(std::string);
  Context execute(ModelController&) override;

 private:
  FilePersistence persistence_;
};

class SaveTasksToFileCommand : public Command {
 public:
  explicit SaveTasksToFileCommand(std::string);
  Context execute(ModelController&) override;

 private:
  FilePersistence persistence_;
};

class VoidCommand : public Command {
 public:
  Context execute(ModelController&) override;
};

}  // namespace task_manager
#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_COMMANDS_COMMANDS_H_
