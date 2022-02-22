#include "interactor/state_machine/commands/Commands.h"
#include "interactor/unit/state_machine/commands/CommandTest.h"

class CompleteTaskCommandTest : public CommandTest {};

TEST_F(CompleteTaskCommandTest, MustCompleteNTasks)
{
    auto _0 = task_factory_.GetNextTask(); // NOLINT
    auto __1 = task_factory_.GetNextTask(); // NOLINT
    auto __2 = task_factory_.GetNextTask(); // NOLINT
    auto ___4 = task_factory_.GetNextTask(); // NOLINT
    auto ___5 = task_factory_.GetNextTask(); // NOLINT
    auto ____6 = task_factory_.GetNextTask(); // NOLINT
    auto _3 = task_factory_.GetNextTask(); // NOLINT
    model_controller_->AddTask(_0);
    model_controller_->AddSubtask(CreateTaskId(0), __1);
    model_controller_->AddSubtask(CreateTaskId(0), __2);
    model_controller_->AddTask(_3);
    model_controller_->AddSubtask(CreateTaskId(2), ___4);
    model_controller_->AddSubtask(CreateTaskId(2), ___5);
    model_controller_->AddSubtask(CreateTaskId(5), ____6);

    std::unordered_map<TaskId, Task::Progress> progresses{
        {CreateTaskId(0), Task::kUncompleted},
        {CreateTaskId(1), Task::kUncompleted},
        {CreateTaskId(2), Task::kCompleted},
        {CreateTaskId(3), Task::kCompleted},
        {CreateTaskId(4), Task::kCompleted},
        {CreateTaskId(5), Task::kCompleted},
        {CreateTaskId(6), Task::kCompleted},
    };
    CompleteTasksCommand command{{CreateTaskId(2), CreateTaskId(3)}};
    auto ctx = command.execute(*model_controller_);
    ASSERT_EQ(ctx.status, ModelController::Status::kOk);
    auto dump = model_controller_->GetAllSolidTasks().AccessResult();
    for (size_t i{0}, sz{dump.size()} ; i != sz ; ++i)
    {
        EXPECT_EQ(dump[i].task().progress(), progresses[dump[i].task_id()]);
    }
}

TEST_F(CompleteTaskCommandTest, MustSetStatusNotPresentId)
{
    auto _0 = task_factory_.GetNextTask();
    model_controller_->AddTask(_0);
    CompleteTasksCommand command({CreateTaskId(0), CreateTaskId(2)});
    auto ctx = command.execute(*model_controller_);
    ASSERT_EQ(ctx.status, ModelController::Status::kNotPresentId);
}