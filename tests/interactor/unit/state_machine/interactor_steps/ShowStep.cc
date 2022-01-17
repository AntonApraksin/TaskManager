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
};

TEST_F(ShowStepTest, FirstCallWithoutArgumentMustReturnGetAllTasksCommand) {
  SetArg("");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<GetAllTasksCommand*>(command.get()), nullptr);
}

TEST_F(ShowStepTest, FirstCallWithArgumentMustReturnGetSpecifiedTasksCommand) {
  SetArg("12 43");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<GetSpecifiedTasksCommand*>(command.get()), nullptr);
}

TEST_F(ShowStepTest,
       FirstCallWithInvalidIdArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("12 qw");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(ShowStepTest,
       FirstCallWithRepeatedIdArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("12 34 12");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(ShowStepTest, SecondCallWithNotPresentIdReturnVoidCommand) {
  SetArg("12 34");
  step_->execute({});
  auto command{
      step_->execute({{}, {}, ModelController::Status::kNotPresentId})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(ShowStepTest, SecondCallWithEmptySolidTasks) {
  SetArg("");
  step_->execute({});
  auto command{step_->execute({{}, {}, ModelController::Status::kOk})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(ShowStepTest, SecondCallWithEverythingOkReturnVoidCommand) {
  SetArg("12 34");
  step_->execute({});
  TaskFactory task_factory;
  SolidTasks solid_tasks{TaskToSolidTask(task_factory.GetNextTask(), 12),
                         TaskToSolidTask(task_factory.GetNextTask(), 34)};
  auto command = step_->execute(
      {{}, std::move(solid_tasks), ModelController::Status::kOk});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(ShowStepTest, MustNotChangeStepAfterFirstExecute) {
  SetArg("12 34");
  step_->execute({});
  std::shared_ptr<Step> to_change{std::make_shared<StepChangeStepTesting>()};
  const auto old_addr = to_change.get();
  step_->ChangeStep(to_change);
  EXPECT_EQ(to_change.get(), old_addr);
}

TEST_F(ShowStepTest, MustChangeStepAfterSecondExecute) {
  SetArg("12 34");
  step_->execute({});
  TaskFactory task_factory;
  SolidTasks solid_tasks{TaskToSolidTask(task_factory.GetNextTask(), 12),
                         TaskToSolidTask(task_factory.GetNextTask(), 34)};
  step_->execute({{}, std::move(solid_tasks), ModelController::Status::kOk});
  std::shared_ptr<Step> to_change{std::make_shared<StepChangeStepTesting>()};
  const auto old_addr = to_change.get();
  step_->ChangeStep(to_change);
  EXPECT_NE(to_change.get(), old_addr);
}
