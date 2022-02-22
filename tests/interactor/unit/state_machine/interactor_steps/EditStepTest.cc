#include "interactor/state_machine/interactor_steps/EditStep.h"

#include "StepTest.h"
#include "test_utils/TaskFactory.h"

class EditStepTest : public StepTest
{
public:
    void SetUp() override { StepTest::SetUp(); }

protected:
    void SetArg(std::string arg)
    {
        step_ = std::make_unique<EditStep>(validator_, io_facility_,
                                           small_step_factory_, std::move(arg));
    }
    std::unique_ptr<EditStep> step_;
    StepParameter step_parameter_;
};

TEST_F(EditStepTest, ExecuteWithoutArgumentMustReturnVoidCommand)
{
    SetArg("");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(EditStepTest, ExecuteWithMultipleArgumentMustReturnVoidCommand)
{
    SetArg("12 43");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(EditStepTest, ExecuteWithInvalidIdArgumentMustReturnVoidCommand)
{
    SetArg("qw");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(EditStepTest, ExecuteWithConfirmationYesReturnEditCommand)
{
    SetArg("3");
    SetInput({"dafs", "", "", "", "y"});
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<EditTaskCommand*>(command.get()), nullptr);
}

TEST_F(EditStepTest, ExecuteWithConfirmatioNoReturnVoidCommand)
{
    SetArg("3");
    SetInput({"dafs", "", "", "", "n"});
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(EditStepTest, SecondCallWithRandomConfirmationMustReturnVoidCommand)
{
    SetArg("3");
    SetInput({"dafs", "", "", "", "dfa"});
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}