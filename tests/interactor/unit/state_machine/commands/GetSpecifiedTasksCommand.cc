#include "interactor/state_machine/commands/Commands.h"
#include "interactor/unit/state_machine/commands/CommandTest.h"

class GetSpecifiedTasksCommandTest : public CommandTest {};

TEST_F(GetSpecifiedTasksCommandTest, MustReturnAllTasks)
{
    auto _0 = task_factory_.GetNextTask();
    auto __1 = task_factory_.GetNextTask();
    auto __2 = task_factory_.GetNextTask();
    auto ___4 = task_factory_.GetNextTask();
    auto ___5 = task_factory_.GetNextTask();
    auto ____6 = task_factory_.GetNextTask();
    auto _3 = task_factory_.GetNextTask();
    model_controller_->Add(_0);
    model_controller_->Add(CreateTaskId(0), __1);
    model_controller_->Add(CreateTaskId(0), __2);
    model_controller_->Add(_3);
    model_controller_->Add(CreateTaskId(2), ___4);
    model_controller_->Add(CreateTaskId(2), ___5);
    model_controller_->Add(CreateTaskId(5), ____6);

    auto dump = model_controller_->GetAllSolidTasks().AccessResult();
    GetSpecifiedTasksCommand command{{CreateTaskId(2), CreateTaskId(3)}};
    auto ctx = command.execute(*model_controller_);
    ASSERT_EQ(ctx.status, ModelController::Status::kOk);
    EXPECT_EQ(
        std::find_if(ctx.solid_tasks->begin(), ctx.solid_tasks->end(),
                     [&sample = _0](auto lhs) { return lhs.task() == sample; }),
        ctx.solid_tasks->end());

    EXPECT_EQ(
        std::find_if(ctx.solid_tasks->begin(), ctx.solid_tasks->end(),
                     [&sample = __1](auto lhs) { return lhs.task() == sample; }),
        ctx.solid_tasks->end());

    EXPECT_NE(
        std::find_if(ctx.solid_tasks->begin(), ctx.solid_tasks->end(),
                     [&sample = __2](auto lhs) { return lhs.task() == sample; }),
        ctx.solid_tasks->end());

    EXPECT_NE(
        std::find_if(ctx.solid_tasks->begin(), ctx.solid_tasks->end(),
                     [&sample = _3](auto lhs) { return lhs.task() == sample; }),
        ctx.solid_tasks->end());

    EXPECT_NE(
        std::find_if(ctx.solid_tasks->begin(), ctx.solid_tasks->end(),
                     [&sample = ___4](auto lhs) { return lhs.task() == sample; }),
        ctx.solid_tasks->end());

    EXPECT_NE(
        std::find_if(ctx.solid_tasks->begin(), ctx.solid_tasks->end(),
                     [&sample = ___5](auto lhs) { return lhs.task() == sample; }),
        ctx.solid_tasks->end());

    EXPECT_NE(std::find_if(
        ctx.solid_tasks->begin(), ctx.solid_tasks->end(),
        [&sample = ____6](auto lhs) { return lhs.task() == sample; }),
              ctx.solid_tasks->end());
}
