#ifndef TASKMANAGER_SRC_REPL_STATE_MACHINE_COMMANDS_COMMANDS_H_
#define TASKMANAGER_SRC_REPL_STATE_MACHINE_COMMANDS_COMMANDS_H_

#include <istream>
#include <memory>
#include <ostream>

#include "interactor/state_machine/commands/CommandResult.h"
#include "persistence/FilePersistence.h"

namespace task_manager
{
class Command
{
public:
    virtual CommandResult execute(ModelController&) = 0;

    virtual ~Command() {};
};

class AddTaskCommand : public Command
{
public:
    explicit AddTaskCommand(Task);
    CommandResult execute(ModelController&) override;

private:
    Task task_;
};

class AddSubtaskCommand : public Command
{
public:
    AddSubtaskCommand(TaskId, Task);
    CommandResult execute(ModelController&) override;

private:
    TaskId task_id_;
    Task task_;
};

class EditTaskCommand : public Command
{
public:
    EditTaskCommand(TaskId, Task);

    CommandResult execute(ModelController&) override;

private:
    TaskId task_id_;
    Task task_;
};

class CompleteTasksCommand : public Command
{
public:
    explicit CompleteTasksCommand(std::vector<TaskId>);

    CommandResult execute(ModelController&) override;

private:
    std::vector<TaskId> task_ids_;
};

class DeleteTasksCommand : public Command
{
public:
    explicit DeleteTasksCommand(std::vector<TaskId>);

    CommandResult execute(ModelController&) override;

private:
    std::vector<TaskId> task_ids_;
};

class GetSpecifiedTasksCommand : public Command
{
public:
    explicit GetSpecifiedTasksCommand(std::vector<TaskId>);

    CommandResult execute(ModelController&) override;

private:
    std::vector<TaskId> task_ids_;
};

class GetAllTasksCommand : public Command
{
public:
    CommandResult execute(ModelController&) override;
};

class LoadTasksCommand : public Command
{
public:
    CommandResult execute(ModelController&) override;
};

class SaveTasksCommand : public Command
{
public:
    CommandResult execute(ModelController&) override;
};

class AddLabelCommand : public Command
{
public:
    AddLabelCommand(TaskId, Label);
    CommandResult execute(ModelController&) override;

private:
    TaskId task_id_;
    Label label_;
};

class DeleteLabelCommand : public Command
{
public:
    DeleteLabelCommand(TaskId, Label);
    CommandResult execute(ModelController&) override;

private:
    TaskId task_id_;
    Label label_;
};

class GetTasksByLabelCommand : public Command
{
public:
    GetTasksByLabelCommand(Label);
    CommandResult execute(ModelController&) override;

private:
    Label label_;
};

class VoidCommand : public Command
{
public:
    CommandResult execute(ModelController&) override;
};

}  // namespace task_manager
#endif  // TASKMANAGER_SRC_REPL_STATE_MACHINE_COMMANDS_COMMANDS_H_
