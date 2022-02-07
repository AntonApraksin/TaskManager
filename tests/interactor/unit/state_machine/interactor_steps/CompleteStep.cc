#include "interactor/state_machine/interactor_steps/CompleteStep.h"

#include "StepTest.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class CompleteStepTest : public StepTest {
 public:
  void SetUp() override { StepTest::SetUp(); }

 protected:
  void SetArg(std::string arg) {
    step_ = std::make_unique<CompleteStep>(validator_, io_facility_,
                                           small_step_factory_, std::move(arg));
  }
  std::unique_ptr<CompleteStep> step_;
  StepParameter step_parameter_;
};

TEST_F(CompleteStepTest, ExecuteWithoutArgumentMustReturnVoidCommand) {
  SetArg("");
  auto command{step_->execute(step_parameter_)};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(CompleteStepTest, ExecuteWithInvalidIdArgumentMustReturnVoidCommand) {
  SetArg("12 qw");
  auto command{step_->execute(step_parameter_)};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(CompleteStepTest, ExecuteWithMultipleArgumentsMustReturnVoidCommand) {
  SetArg("12 34");
  auto command{step_->execute(step_parameter_)};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(CompleteStepTest, ExecuteWithConfirmationYesReturnCompleteCommand) {
  SetArg("1");
  SetInput({"y"});
  auto command = step_->execute(step_parameter_);
  EXPECT_NE(dynamic_cast<CompleteTasksCommand*>(command.get()), nullptr);
}

TEST_F(CompleteStepTest, ExecuteWithConfirmationNoReturnCompleteCommand) {
  SetArg("1");
  SetInput({"n"});
  auto command = step_->execute(step_parameter_);
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(CompleteStepTest,
       ExecuteTaskWithRandomConfirmationMustReturnVoidCommand) {
  SetArg("1");
  SetInput({"gdf"});
  auto command = step_->execute(step_parameter_);
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}