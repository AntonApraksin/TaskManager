#include "interactor/state_machine/commands/Commands.h"
#include "interactor/unit/state_machine/commands/CommandTest.h"

class LoadTasksCommandTest : public CommandTest {};

TEST_F(LoadTasksCommandTest, MustLoadWithOk)
{
    EXPECT_CALL(*mock_persistence_, Load())
        .WillOnce(testing::Return(
            OperationResult<Persistence::Status, SolidTasks>::Ok({})));
    LoadTasksCommand command;
    auto result = command.execute(*model_controller_);
    ASSERT_EQ(result.status, ModelController::Status::kOk);
}

TEST_F(LoadTasksCommandTest, MustLoadWithSaveFailure)
{
    EXPECT_CALL(*mock_persistence_, Load())
        .WillOnce(testing::Return(
            OperationResult<Persistence::Status, SolidTasks>::Error(Persistence::Status::kFailureReading)));
    LoadTasksCommand command;
    auto result = command.execute(*model_controller_);
    ASSERT_EQ(result.status, ModelController::Status::kLoadFailure);
}
