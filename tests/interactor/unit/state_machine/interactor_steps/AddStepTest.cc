#include "interactor/state_machine/interactor_steps/AddStep.h"
#include "interactor/state_machine/interactor_steps/FinalizeStep.h"

#include "StepTest.h"
#include "test_utils/TaskFactory.h"

class AddStepTest : public StepTest
{
public:
    void SetUp() override { StepTest::SetUp(); }

protected:
    void SetArg(std::string arg)
    {
        step_ = std::make_unique<AddStep>(validator_, io_facility_,
                                          small_step_factory_, std::move(arg));
    }
    std::unique_ptr<AddStep> step_;
    StepParameter step_parameter_;
};

TEST_F(AddStepTest, ExecuteWithMultipleArgumentedMustReturnVoidCommand)
{
    SetArg("12 12");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddStepTest, ExecuteWithInvalidArgrumentMustReturnVoidCommand)
{
    SetArg("foo");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddStepTest, ExecuteWithoutArgumentMustReturnAddTaskCommand)
{
    SetArg("");
    SetInput({"dafs", "", "", "", "y"});
    auto command = step_->execute(step_parameter_);
    EXPECT_NE(dynamic_cast<AddTaskCommand*>(command.get()), nullptr);
}

TEST_F(AddStepTest, ExecuteWithArgumentMustReturnAddSubtaskTaskCommand)
{
    SetArg("1");
    SetInput({"dafs", "", "", "", "y"});
    auto command = step_->execute(step_parameter_);
    EXPECT_NE(dynamic_cast<AddSubtaskCommand*>(command.get()), nullptr);
}

TEST_F(AddStepTest, ExecuteTaskWithRandomConfirmationMustReturnVoidCommand)
{
    SetArg("");
    SetInput({"dafs", "", "", "", "gdf"});
    auto command = step_->execute(step_parameter_);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddStepTest, ExecuteSubtaskWithRandomConfirmationMustReturnVoidCommand)
{
    SetArg("1");
    SetInput({"dafs", "", "", "", "gdf"});
    auto command = step_->execute(step_parameter_);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddStepTest, ExecuteTaskWithConfirmationNoMustReturnVoidCommand)
{
    SetArg("");
    SetInput({"dafs", "", "", "", "n"});
    auto command = step_->execute(step_parameter_);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddStepTest, ExecuteSubtaskWithConfirmationNoMustReturnVoidCommand)
{
    SetArg("1");
    SetInput({"dafs", "", "", "", "n"});
    auto command = step_->execute(step_parameter_);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddStepTest, ChangeStepMustChangeToFinalizeStep)
{
    SetArg("");
    auto changed_step = step_->ChangeStep();
    EXPECT_NE(dynamic_cast<FinalizeStep*>(changed_step.get()), nullptr);
}
