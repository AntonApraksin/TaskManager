#include "interactor/state_machine/commands/Commands.h"
#include "interactor/unit/state_machine/commands/CommandTest.h"

class GetAllTasksCommandTest : public CommandTest {};

TEST_F(GetAllTasksCommandTest, MustReturnAllTasks)
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

    auto dump = model_controller_->GetAllSolidTasks().AccessResult();
    GetAllTasksCommand command;
    auto ctx = command.execute(*model_controller_);
    ASSERT_EQ(ctx.status, ModelController::Status::kOk);
    ASSERT_EQ(dump.size(), ctx.solid_tasks->size());

    for (size_t i{0}, sz{dump.size()} ; i != sz ; ++i)
    {
        EXPECT_EQ(dump[i], (*ctx.solid_tasks)[i]);
    }
}
