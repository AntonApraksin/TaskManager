#include "interactor/state_machine/commands/Commands.h"
#include "interactor/unit/state_machine/commands/CommandTest.h"

class SaveTasksCommandTest : public CommandTest {};

TEST_F(SaveTasksCommandTest, MustSaveWithOk)
{
    EXPECT_CALL(*mock_persistence_, Save(testing::_))
        .WillOnce(testing::Return(
            OperationResult<Persistence::Status>::Ok()));
    SaveTasksCommand command;
    auto result = command.execute(*model_controller_);
    ASSERT_EQ(result.status, ModelController::Status::kOk);
}

TEST_F(SaveTasksCommandTest, MustSaveWithSaveFailure)
{
    EXPECT_CALL(*mock_persistence_, Save(testing::_))
        .WillOnce(testing::Return(
            OperationResult<Persistence::Status>::Error(Persistence::Status::kFailureWriting)));
    SaveTasksCommand command;
    auto result = command.execute(*model_controller_);
    ASSERT_EQ(result.status, ModelController::Status::kSaveFailure);
}
