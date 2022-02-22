#include "interactor/state_machine/interactor_steps/LoadStep.h"

#include "StepTest.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class LoadStepTest : public StepTest
{
public:
    void SetUp() override { StepTest::SetUp(); }

protected:
    void SetArg(std::string arg)
    {
        step_ = std::make_unique<LoadStep>(validator_, io_facility_,
                                           small_step_factory_, std::move(arg));
    }
    std::unique_ptr<LoadStep> step_;
    StepParameter step_parameter_;
};

TEST_F(LoadStepTest, ExecuteWithoutArgumentReturnLoadTasksCommand)
{
    SetArg("");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<LoadTasksCommand*>(command.get()), nullptr);
}