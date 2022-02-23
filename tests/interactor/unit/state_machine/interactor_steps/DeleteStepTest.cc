#include "interactor/state_machine/interactor_steps/DeleteStep.h"
#include "interactor/state_machine/interactor_steps/FinalizeStep.h"

#include "StepTest.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class DeleteStepTest : public StepTest
{
public:
    void SetUp() override { StepTest::SetUp(); }

protected:
    void SetArg(std::string arg)
    {
        step_ = std::make_unique<DeleteStep>(validator_, io_facility_,
                                             small_step_factory_, std::move(arg));
    }
    std::unique_ptr<DeleteStep> step_;
    StepParameter step_parameter_;
};

TEST_F(DeleteStepTest, ExecuteWithoutArgumentMustReturnVoidCommand)
{
    SetArg("");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(DeleteStepTest, ExecuteWithInvalidIdMustReturnVoidCommand)
{
    SetArg("foo");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(DeleteStepTest, ExecuteWithInvalidIdArgumentMustReturnVoidCommand)
{
    SetArg("12 qw");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(DeleteStepTest, ExecuteWithMultipleArgumentsMustReturnVoidCommand)
{
    SetArg("12 34");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(DeleteStepTest, ExecuteWithConfirmationYesReturnDeleteCommand)
{
    SetArg("1");
    SetInput({"y"});
    auto command = step_->execute(step_parameter_);
    EXPECT_NE(dynamic_cast<DeleteTasksCommand*>(command.get()), nullptr);
}

TEST_F(DeleteStepTest, ExecuteWithConfirmationNoReturnCompleteCommand)
{
    SetArg("1");
    SetInput({"n"});
    auto command = step_->execute(step_parameter_);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(DeleteStepTest, ExecuteTaskWithRandomConfirmationMustReturnVoidCommand)
{
    SetArg("1");
    SetInput({"gdf"});
    auto command = step_->execute(step_parameter_);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(DeleteStepTest, ChangeStepMustChangeToFinalizeStep)
{
    SetArg("");
    auto changed_step = step_->ChangeStep();
    EXPECT_NE(dynamic_cast<FinalizeStep*>(changed_step.get()), nullptr);
}
