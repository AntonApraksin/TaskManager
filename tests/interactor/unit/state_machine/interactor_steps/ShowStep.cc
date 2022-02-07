#include "interactor/state_machine/interactor_steps/ShowStep.h"

#include "StepTest.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class ShowStepTest : public StepTest {
 public:
  void SetUp() override { StepTest::SetUp(); }

 protected:
  void SetArg(std::string arg) {
    step_ = std::make_unique<ShowStep>(validator_, io_facility_,
                                       small_step_factory_, std::move(arg));
  }
  std::unique_ptr<ShowStep> step_;
  StepParameter step_parameter_;
};

TEST_F(ShowStepTest, ExecuteWithoutArgumentMustReturnGetAllTasksCommand) {
  SetArg("");
  auto command{step_->execute(step_parameter_)};
  EXPECT_NE(dynamic_cast<GetAllTasksCommand*>(command.get()), nullptr);
}

TEST_F(ShowStepTest, ExecuteWithArgumentMustReturnGetSpecifiedTasksCommand) {
  SetArg("12 43");
  auto command{step_->execute(step_parameter_)};
  EXPECT_NE(dynamic_cast<GetSpecifiedTasksCommand*>(command.get()), nullptr);
}

TEST_F(ShowStepTest, ExecuteWithInvalidIdArgumentMustReturnVoidCommand) {
  SetArg("12 qw");
  auto command{step_->execute(step_parameter_)};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}