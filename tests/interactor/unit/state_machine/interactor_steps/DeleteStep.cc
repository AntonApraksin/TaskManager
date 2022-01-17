#include "interactor/state_machine/interactor_steps/DeleteStep.h"

#include "StepTest.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class DeleteStepTest : public StepTest {
 public:
  void SetUp() override { StepTest::SetUp(); }

 protected:
  void SetArg(std::string arg) {
    step_ = std::make_unique<DeleteStep>(validator_, io_facility_,
                                         small_step_factory_, std::move(arg));
  }
  std::unique_ptr<DeleteStep> step_;
};

TEST_F(DeleteStepTest,
       FirstCallWithoutArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(DeleteStepTest,
       FirstCallWithArgumentMustReturnGetSpecifiedTasksCommand) {
  SetArg("12 43");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<GetSpecifiedTasksCommand*>(command.get()), nullptr);
}

TEST_F(DeleteStepTest,
       FirstCallWithInvalidIdArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("12 qw");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(DeleteStepTest,
       FirstCallWithRepeatedIdArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("12 34 12");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(DeleteStepTest, SecondCallWithoutSolidTasksReturnVoidCommand) {
  SetArg("12 34");
  step_->execute({});
  auto command{
      step_->execute({{}, {}, ModelController::Status::kNotPresentId})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(DeleteStepTest,
       SecondCallWithSolidTasksAndConfirmationYesReturnDeleteCommand) {
  SetArg("1");
  SetInput({"y"});
  step_->execute({});
  TaskFactory task_factory;
  auto solid_task{TaskToSolidTask(task_factory.GetNextTask(), 1)};
  auto command = step_->execute(
      {{}, SolidTasks{std::move(solid_task)}, ModelController::Status::kOk});
  EXPECT_NE(dynamic_cast<DeleteTasksCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(DeleteStepTest,
       SecondCallWithParentChildRelationShipMustReturnVoidCommand) {
  SetArg("1 2");
  SetInput({"y"});
  step_->execute({});
  TaskFactory task_factory;
  SolidTasks solid_tasks{TaskToSolidTask(task_factory.GetNextTask(), 1),
                         TaskToSolidTask(task_factory.GetNextTask(), 2, 1)};
  auto command = step_->execute(
      {{}, std::move(solid_tasks), ModelController::Status::kOk});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(DeleteStepTest,
       SecondCallTaskWithRandomConfirmationMustReturnVoidCommand) {
  SetArg("1");
  SetInput({"gdf"});
  step_->execute({});
  TaskFactory task_factory;
  auto solid_task{TaskToSolidTask(task_factory.GetNextTask(), 1)};
  auto command = step_->execute(
      {{}, SolidTasks{std::move(solid_task)}, ModelController::Status::kOk});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(DeleteStepTest, SecondCallTaskWithConfirmationNoMustReturnVoidCommand) {
  SetArg("1");
  SetInput({"n"});
  step_->execute({});
  TaskFactory task_factory;
  auto solid_task{TaskToSolidTask(task_factory.GetNextTask(), 1)};
  auto command = step_->execute(
      {{}, SolidTasks{std::move(solid_task)}, ModelController::Status::kOk});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(DeleteStepTest, MustNotChangeStepAfterFirstExecute) {
  SetArg("1");
  step_->execute({});
  std::shared_ptr<Step> to_change{std::make_shared<StepChangeStepTesting>()};
  const auto old_addr = to_change.get();
  step_->ChangeStep(to_change);
  EXPECT_EQ(to_change.get(), old_addr);
}

TEST_F(DeleteStepTest, MustChangeStepAfterSecondExecute) {
  SetArg("1");
  step_->execute({});
  SetInput({"y"});
  TaskFactory task_factory;
  auto solid_task{TaskToSolidTask(task_factory.GetNextTask(), 1)};
  step_->execute(
      {{}, SolidTasks{std::move(solid_task)}, ModelController::Status::kOk});
  std::shared_ptr<Step> to_change{std::make_shared<StepChangeStepTesting>()};
  const auto old_addr = to_change.get();
  step_->ChangeStep(to_change);
  EXPECT_NE(to_change.get(), old_addr);
}
