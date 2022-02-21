#include "interactor/state_machine/interactor_steps/UnknownStep.h"

#include "StepTest.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class UnknownStepTest : public StepTest
{
public:
    void SetUp() override { StepTest::SetUp(); }

protected:
    void SetArg(std::string arg)
    {
        step_ = std::make_unique<UnknownStep>(validator_, io_facility_,
                                              small_step_factory_, std::move(arg));
    }
    std::unique_ptr<UnknownStep> step_;
    StepParameter step_parameter_;
};

TEST_F(UnknownStepTest, ExecuteWithoutArgumentMustReturnVoidCommmand)
{
    SetArg("");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(UnknownStepTest, ExecuteWithArgumentMustReturnVoidCommmand)
{
    SetArg("sfsad");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}