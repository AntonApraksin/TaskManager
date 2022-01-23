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

#define IT_WAS_NECESARRY_PLS_DONT_KILL_ME(StepName, long_name)    \
  TEST_F(PromptStepTest, ChangeStepMustSet##StepName) {           \
    SetArg("");                                                   \
    SetInput({long_name});                                        \
    std::shared_ptr<Step> to_change;                              \
    step_->ChangeStep(to_change);                                 \
    EXPECT_NE(dynamic_cast<StepName*>(to_change.get()), nullptr); \
    to_change.reset();                                            \
    SetInput({long_name " 12 eeqw 43ew"});                        \
    step_->ChangeStep(to_change);                                 \
    EXPECT_NE(dynamic_cast<StepName*>(to_change.get()), nullptr); \
    to_change.reset();                                            \
  }

class PromptStepTest : public StepTest {
 public:
  void SetUp() override { StepTest::SetUp(); }

 protected:
  void SetArg(std::string) {
    step_ = std::make_unique<PromptStep>(validator_, io_facility_,
                                         small_step_factory_);
  }
  std::unique_ptr<PromptStep> step_;
  StepParameter step_parameter_;
};

TEST_F(PromptStepTest, ExecuteReturnVoidCommand) {
  SetArg("");
  auto command{step_->execute(step_parameter_)};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

IT_WAS_NECESARRY_PLS_DONT_KILL_ME(AddStep, "add")
IT_WAS_NECESARRY_PLS_DONT_KILL_ME(EditStep, "edit")
IT_WAS_NECESARRY_PLS_DONT_KILL_ME(CompleteStep, "complete")
IT_WAS_NECESARRY_PLS_DONT_KILL_ME(DeleteStep, "delete")
IT_WAS_NECESARRY_PLS_DONT_KILL_ME(ShowStep, "show")
IT_WAS_NECESARRY_PLS_DONT_KILL_ME(SaveStep, "save")
IT_WAS_NECESARRY_PLS_DONT_KILL_ME(LoadStep, "load")
IT_WAS_NECESARRY_PLS_DONT_KILL_ME(UnknownStep, "gfdskghd")
IT_WAS_NECESARRY_PLS_DONT_KILL_ME(HelpStep, "help")

TEST_F(PromptStepTest, ChangeStepMustSetNullptr) {
  SetArg("");
  SetInput({"quit"});
  std::shared_ptr<Step> to_change;
  step_->ChangeStep(to_change);
  EXPECT_EQ(to_change, nullptr);
  to_change.reset();
  SetInput({"quit 12 eeqw 43ew"});
  step_->ChangeStep(to_change);
  EXPECT_EQ(to_change, nullptr);
  to_change.reset();
}
