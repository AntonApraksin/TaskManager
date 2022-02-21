#include "interactor/state_machine/interactor_steps/SaveStep.h"

#include "StepTest.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class SaveStepTest : public StepTest
{
public:
    void SetUp() override { StepTest::SetUp(); }

protected:
    void SetArg(std::string arg)
    {
        step_ = std::make_unique<SaveStep>(validator_, io_facility_,
                                           small_step_factory_, std::move(arg));
    }
    std::unique_ptr<SaveStep> step_;
    StepParameter step_parameter_;
};

TEST_F(SaveStepTest, ExecuteWithoutArgumentReturnSaveTasksCommand)
{
    SetArg("");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<SaveTasksCommand*>(command.get()), nullptr);
}