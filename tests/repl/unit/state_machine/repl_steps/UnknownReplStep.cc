#include "repl/state_machine/repl_steps/UnknownReplStep.h"

#include "ReplStepTest.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class UnknownReplStepTest : public ReplStepTest {
 public:
  void SetUp() override { ReplStepTest::SetUp(); }

 protected:
  void SetArg(std::string arg) {
    step_ = std::make_unique<UnknownReplStep>(
        validator_, io_facility_, small_step_factory_, std::move(arg));
  }
  std::unique_ptr<UnknownReplStep> step_;
};

TEST_F(UnknownReplStepTest, WithoutArgumentMustReturnVoidCommmand) {
  SetArg("");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(UnknownReplStepTest, WithArgumentMustReturnVoidCommmand) {
  SetArg("sfsad");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}
