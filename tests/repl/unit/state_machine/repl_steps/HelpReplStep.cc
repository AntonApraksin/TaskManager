#include "repl/state_machine/repl_steps/HelpReplStep.h"

#include "ReplStepTest.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class HelpReplStepTest : public ReplStepTest {
 public:
  void SetUp() override { ReplStepTest::SetUp(); }

 protected:
  void SetArg(std::string arg) {
    step_ = std::make_unique<HelpReplStep>(validator_, io_facility_,
                                           small_step_factory_, std::move(arg));
  }
  std::unique_ptr<HelpReplStep> step_;
};

TEST_F(HelpReplStepTest, WithoutArgumentMustReturnVoidCommmand) {
  SetArg("");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(HelpReplStepTest, WithArgumentMustReturnVoidCommmand) {
  SetArg("sfsad");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}
