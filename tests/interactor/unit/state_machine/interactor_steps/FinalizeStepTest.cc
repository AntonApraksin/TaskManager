#include "interactor/state_machine/interactor_steps/FinalizeStep.h"
#include "interactor/state_machine/interactor_steps/PromptStep.h"
#include "utils/TaskIdUtils.h"

#include "StepTest.h"
#include "test_utils/TaskFactory.h"

class FinalizeStepTest : public StepTest
{
public:
    void SetUp() override { StepTest::SetUp(); }

protected:
    void SetArg(std::string)
    {
        step_ = std::make_unique<FinalizeStep>(validator_, io_facility_,
                                          small_step_factory_);
    }
    std::unique_ptr<FinalizeStep> step_;
    StepParameter step_parameter_;
};

TEST_F(FinalizeStepTest, ExecuteMustReturnVoidCommandWithKShowId)
{
    SetArg("");
    Context ctx;
    ctx.event = task_manager::StepEvent::kShowId;
    ctx.task_id = CreateTaskId(0);
    StepParameter parameter;
    parameter.ctx = ctx;
    auto command = step_->execute(parameter);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(FinalizeStepTest, ExecuteMustReturnVoidCommandWithKSaveFailure)
{
    SetArg("");
    Context ctx;
    ctx.event = task_manager::StepEvent::kSaveFailure;
    StepParameter parameter;
    parameter.ctx = ctx;
    auto command = step_->execute(parameter);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(FinalizeStepTest, ExecuteMustReturnVoidCommandWithKLoadFailure)
{
    SetArg("");
    Context ctx;
    ctx.event = task_manager::StepEvent::kLoadFailure;
    StepParameter parameter;
    parameter.ctx = ctx;
    auto command = step_->execute(parameter);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(FinalizeStepTest, ExecuteMustReturnVoidCommandWithKNotPresentLabel)
{
    SetArg("");
    Context ctx;
    ctx.event = task_manager::StepEvent::kNotPresentLabel;
    StepParameter parameter;
    parameter.ctx = ctx;
    auto command = step_->execute(parameter);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(FinalizeStepTest, ExecuteMustReturnVoidCommandWithKNotPresentId)
{
    SetArg("");
    Context ctx;
    ctx.event = task_manager::StepEvent::kNotPresentId;
    StepParameter parameter;
    parameter.ctx = ctx;
    auto command = step_->execute(parameter);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(FinalizeStepTest, ExecuteMustReturnVoidCommandWithKNothing)
{
    SetArg("");
    Context ctx;
    ctx.event = task_manager::StepEvent::kNothing;
    StepParameter parameter;
    parameter.ctx = ctx;
    auto command = step_->execute(parameter);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(FinalizeStepTest, ExecuteMustReturnVoidCommandWithKShowAll)
{
    SetArg("");
    Context ctx;
    ctx.event = task_manager::StepEvent::kShowAll;
    ctx.solid_tasks = SolidTasks{};
    StepParameter parameter;
    parameter.ctx = ctx;
    auto command = step_->execute(parameter);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(FinalizeStepTest, ExecuteMustReturnVoidCommandWithKShowWithoutNest)
{
    SetArg("");
    Context ctx;
    ctx.event = task_manager::StepEvent::kShowWithoutNest;
    ctx.solid_tasks = SolidTasks{};
    StepParameter parameter;
    parameter.ctx = ctx;
    auto command = step_->execute(parameter);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(FinalizeStepTest, ExecuteMustReturnVoidCommandWithKShowById)
{
    SetArg("");
    Context ctx;
    ctx.event = task_manager::StepEvent::kShowById;
    ctx.solid_tasks = SolidTasks{};
    StepParameter parameter;
    parameter.ctx = ctx;
    auto command = step_->execute(parameter);
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(FinalizeStepTest, ChangeStepMustReturnPromptStep)
{
    SetArg("");
    auto next_step = step_->ChangeStep();
    EXPECT_NE(dynamic_cast<PromptStep*>(next_step.get()), nullptr);
}
