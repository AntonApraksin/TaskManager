#include "repl/state_machine/repl_steps/PromptReplStep.h"

#include "ReplStepTest.h"
#include "repl/state_machine/repl_steps/AddReplStep.h"
#include "repl/state_machine/repl_steps/CompleteReplStep.h"
#include "repl/state_machine/repl_steps/DeleteReplStep.h"
#include "repl/state_machine/repl_steps/EditReplStep.h"
#include "repl/state_machine/repl_steps/HelpReplStep.h"
#include "repl/state_machine/repl_steps/LoadReplStep.h"
#include "repl/state_machine/repl_steps/SaveReplStep.h"
#include "repl/state_machine/repl_steps/ShowReplStep.h"
#include "repl/state_machine/repl_steps/UnknownReplStep.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

#define IT_WAS_NECESARRY_PLS_DONT_KILL_ME(StepName, long_name)    \
  TEST_F(PromptReplStepTest, ChangeStepMustSet##StepName) {       \
    SetArg("");                                                   \
    SetInput({long_name});                                        \
    std::shared_ptr<ReplStep> to_change;                          \
    step_->ChangeStep(to_change);                                 \
    EXPECT_NE(dynamic_cast<StepName*>(to_change.get()), nullptr); \
    to_change.reset();                                            \
    SetInput({long_name " 12 eeqw 43ew"});                        \
    step_->ChangeStep(to_change);                                 \
    EXPECT_NE(dynamic_cast<StepName*>(to_change.get()), nullptr); \
    to_change.reset();                                            \
  }

class PromptReplStepTest : public ReplStepTest {
 public:
  void SetUp() override { ReplStepTest::SetUp(); }

 protected:
  void SetArg(std::string) {
    step_ = std::make_unique<PromptReplStep>(validator_, io_facility_,
                                             small_step_factory_);
  }
  std::unique_ptr<PromptReplStep> step_;
};

TEST_F(PromptReplStepTest, ExecuteReturnVoidCommand) {
  SetArg("");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

IT_WAS_NECESARRY_PLS_DONT_KILL_ME(AddReplStep, "add")
IT_WAS_NECESARRY_PLS_DONT_KILL_ME(EditReplStep, "edit")
IT_WAS_NECESARRY_PLS_DONT_KILL_ME(CompleteReplStep, "complete")
IT_WAS_NECESARRY_PLS_DONT_KILL_ME(DeleteReplStep, "delete")
IT_WAS_NECESARRY_PLS_DONT_KILL_ME(ShowReplStep, "show")
IT_WAS_NECESARRY_PLS_DONT_KILL_ME(SaveReplStep, "save")
IT_WAS_NECESARRY_PLS_DONT_KILL_ME(LoadReplStep, "load")
IT_WAS_NECESARRY_PLS_DONT_KILL_ME(UnknownReplStep, "gfdskghd")
IT_WAS_NECESARRY_PLS_DONT_KILL_ME(HelpReplStep, "help")

TEST_F(PromptReplStepTest, ChangeStepMustSetNullptr) {
  SetArg("");
  SetInput({"quit"});
  std::shared_ptr<ReplStep> to_change;
  step_->ChangeStep(to_change);
  EXPECT_EQ(to_change, nullptr);
  to_change.reset();
  SetInput({"quit 12 eeqw 43ew"});
  step_->ChangeStep(to_change);
  EXPECT_EQ(to_change, nullptr);
  to_change.reset();
}
