#include "interactor/state_machine/interactor_steps/AddLabelStep.h"

#include "StepTest.h"
#include "interactor/state_machine/interactor_steps/FinalizeStep.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class AddLabelStepTest : public StepTest
{
public:
    void SetUp() override { StepTest::SetUp(); }

protected:
    void SetArg(std::string arg)
    {
        step_ = std::make_unique<AddLabelStep>(validator_, io_facility_,
                                               small_step_factory_, std::move(arg));
    }
    StepParameter step_parameter_;
    std::unique_ptr<AddLabelStep> step_;
};

TEST_F(AddLabelStepTest, ExecuteWithoutArgumentMustReturnVoidCommand)
{
    SetArg("");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddLabelStepTest, ExecuteWithIdButWithoutLabelMustReturnVoidCommand)
{
    SetArg("12");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddLabelStepTest, ExecuteWithInvalidIdAndLabelMustReturnVoidCommand)
{
    SetArg("a foo");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddLabelStepTest, ExecuteWithThirdArgumentMustReturnVoidCommand)
{
    SetArg("12 foo 34");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddLabelStepTest, ExecuteWithRandomConfirmationMustReturnVoidCommand)
{
    SetArg("12 foo");
    SetInput({"dfa"});
    auto command = step_->execute(step_parameter_);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddLabelStepTest, SecondCallWithConfirmationNoMustReturnVoidCommand)
{
    SetArg("12 foo");
    SetInput({"n"});
    auto command = step_->execute(step_parameter_);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddLabelStepTest, ExecuteWithIdAndLabelMustReturnAddLabelCommand)
{
    SetArg("12 foo");
    SetInput({"y"});
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<AddLabelCommand*>(command.get()), nullptr);
}

TEST_F(AddLabelStepTest, ChangeStepMustChangeToFinalizeStep)
{
    SetArg("12 foo");
    auto changed_step = step_->ChangeStep();
    EXPECT_NE(dynamic_cast<FinalizeStep*>(changed_step.get()), nullptr);
}