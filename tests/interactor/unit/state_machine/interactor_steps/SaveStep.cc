#include "interactor/state_machine/interactor_steps/SaveStep.h"

#include "StepTest.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class SaveStepTest : public StepTest {
 public:
  void SetUp() override { StepTest::SetUp(); }

 protected:
  void SetArg(std::string arg) {
    step_ = std::make_unique<SaveStep>(validator_, io_facility_,
                                       small_step_factory_, std::move(arg));
  }
  std::unique_ptr<SaveStep> step_;
};

TEST_F(SaveStepTest, MustNotChangeStepAfterFirstExecute) {
  SetArg("foo.txt");
  step_->execute({});
  std::shared_ptr<Step> to_change{std::make_shared<StepChangeStepTesting>()};
  const auto old_addr = to_change.get();
  step_->ChangeStep(to_change);
  EXPECT_EQ(to_change.get(), old_addr);
}

TEST_F(SaveStepTest, MustChangeStepAfterSecondExecute) {
  SetArg("foo.txt");
  step_->execute({});
  step_->execute({{}, {}, ModelController::Status::kOk});
  std::shared_ptr<Step> to_change{std::make_shared<StepChangeStepTesting>()};
  const auto old_addr = to_change.get();
  step_->ChangeStep(to_change);
  EXPECT_NE(to_change.get(), old_addr);
}

// TODO: How to test files?
