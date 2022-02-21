#include "interactor/state_machine/interactor_steps/PromptStep.h"

#include "StepTest.h"
#include "interactor/state_machine/interactor_steps/AddStep.h"
#include "interactor/state_machine/interactor_steps/CompleteStep.h"
#include "interactor/state_machine/interactor_steps/DeleteStep.h"
#include "interactor/state_machine/interactor_steps/EditStep.h"
#include "interactor/state_machine/interactor_steps/HelpStep.h"
#include "interactor/state_machine/interactor_steps/LoadStep.h"
#include "interactor/state_machine/interactor_steps/SaveStep.h"
#include "interactor/state_machine/interactor_steps/ShowStep.h"
#include "interactor/state_machine/interactor_steps/UnknownStep.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

#define GENERATE_TEST(StepName, long_name)       \
  TEST_F(PromptStepTest, ChangeStepMustSet##StepName) {              \
    SetArg("");                                                      \
    SetInput({long_name});                                           \
    auto changed_step = step_->ChangeStep();                         \
    EXPECT_NE(dynamic_cast<StepName*>(changed_step.get()), nullptr); \
    SetInput({long_name " 12 eeqw 43ew"});                           \
    changed_step = step_->ChangeStep();                              \
    EXPECT_NE(dynamic_cast<StepName*>(changed_step.get()), nullptr); \
  }

class PromptStepTest : public StepTest
{
public:
    void SetUp() override { StepTest::SetUp(); }

protected:
    void SetArg(std::string)
    {
        step_ = std::make_unique<PromptStep>(validator_, io_facility_,
                                             small_step_factory_);
    }
    std::unique_ptr<PromptStep> step_;
    StepParameter step_parameter_;
};

TEST_F(PromptStepTest, ExecuteReturnVoidCommand)
{
    SetArg("");
    auto command{step_->execute(step_parameter_)};
    EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

GENERATE_TEST(AddStep, "add")
GENERATE_TEST(EditStep, "edit")
GENERATE_TEST(CompleteStep, "complete")
GENERATE_TEST(DeleteStep, "delete")
GENERATE_TEST(ShowStep, "show")
GENERATE_TEST(SaveStep, "save")
GENERATE_TEST(LoadStep, "load")
GENERATE_TEST(UnknownStep, "gfdskghd")
GENERATE_TEST(HelpStep, "help")

TEST_F(PromptStepTest, ChangeStepMustSetNullptr)
{
    SetArg("");
    SetInput({"quit"});
    std::shared_ptr<Step> to_change;
    step_->ChangeStep();
    EXPECT_EQ(to_change, nullptr);
    to_change.reset();
    SetInput({"quit 12 eeqw 43ew"});
    step_->ChangeStep();
    EXPECT_EQ(to_change, nullptr);
    to_change.reset();
}
