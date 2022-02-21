#include "interactor/state_machine/interactor_steps/DeleteLabelStep.h"

#include "StepTest.h"
#include "interactor/state_machine/interactor_steps/FinalizeStep.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class DeleteLabelStepTest : public StepTest
{
public:
    void SetUp() override { StepTest::SetUp(); }

protected:
    void SetArg(std::string arg)
    {
        step_ = std::make_unique<DeleteLabelStep>(
            validator_, io_facility_, small_step_factory_, std::move(arg));
    }
    std::unique_ptr<DeleteLabelStep> step_;
    StepParameter step_parameter_;
};

TEST_F(DeleteLabelStepTest, ExecuteWithoutArgumentMustReturnVoidCommand)
{
    SetArg("");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(DeleteLabelStepTest, ExecuteWithIdButWithoutLabelMustReturnVoidCommand)
{
    SetArg("12");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(DeleteLabelStepTest, ExecuteWithInvalidIdAndLabelMustReturnVoidCommand)
{
    SetArg("a foo");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(DeleteLabelStepTest, ExecuteWithThirdArgumentMustReturnVoidCommand)
{
    SetArg("12 foo 34");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(DeleteLabelStepTest,
       ExecuteWithRandomConfirmationMustReturnVoidCommand)
{
    SetArg("12 foo");
    SetInput({"dfa"});
    auto command = step_->execute(step_parameter_);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(DeleteLabelStepTest, SecondCallWithConfirmationNoMustReturnVoidCommand)
{
    SetArg("12 foo");
    SetInput({"n"});
    auto command = step_->execute(step_parameter_);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(DeleteLabelStepTest, ExecuteWithIdAndLabelMustReturnDeleteLabelCommand)
{
    SetArg("12 foo");
    SetInput({"y"});
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<DeleteLabelCommand*>(command.get()), nullptr);
}

TEST_F(DeleteLabelStepTest, ChangeStepMustChangeToFinalizeStep)
{
    SetArg("12 foo");
    auto changed_step = step_->ChangeStep();
    EXPECT_NE(dynamic_cast<FinalizeStep*>(changed_step.get()), nullptr);
}