#include "ReplStepTest.h"
#include "repl/state_machine/repl_steps/LoadReplStep.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class LoadReplStepTest : public ReplStepTest {
 public:
  void SetUp() override { ReplStepTest::SetUp(); }

 protected:
  void SetArg(std::string arg) {
    step_ = std::make_unique<LoadReplStep>(validator_, io_facility_,
                                           small_step_factory_, std::move(arg));
  }
  std::unique_ptr<LoadReplStep> step_;
};

TEST_F(LoadReplStepTest,
       FirstCallWithoutArgumentReturnVoidCommandAndThenCauseDeath) {
  SetArg("");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

// TODO: How to test files?