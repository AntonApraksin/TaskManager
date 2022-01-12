#include "repl/state_machine/repl_steps/CompleteReplStep.h"

#include "ReplStepTest.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class CompleteReplStepTest : public ReplStepTest {
 public:
  void SetUp() override { ReplStepTest::SetUp(); }

 protected:
  void SetArg(std::string arg) {
    step_ = std::make_unique<CompleteReplStep>(
        validator_, io_facility_, small_step_factory_, std::move(arg));
  }
  std::unique_ptr<CompleteReplStep> step_;
};

TEST_F(CompleteReplStepTest,
       FirstCallWithoutArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(CompleteReplStepTest,
       FirstCallWithArgumentMustReturnGetSpecifiedTasksCommand) {
  SetArg("12 43");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<GetSpecifiedTasksCommand*>(command.get()), nullptr);
}

TEST_F(CompleteReplStepTest,
       FirstCallWithInvalidIdArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("12 qw");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(CompleteReplStepTest,
       FirstCallWithRepeatedIdArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("12 34 12");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(CompleteReplStepTest, SecondCallWithoutSolidTasksReturnVoidCommand) {
  SetArg("12 34");
  step_->execute({});
  auto command{
      step_->execute({{}, {}, ModelController::Status::kNotPresentId})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(CompleteReplStepTest,
       SecondCallWithSolidTasksAndConfirmationYesReturnCompleteCommand) {
  SetArg("1");
  SetInput({"y"});
  step_->execute({});
  TaskFactory task_factory;
  auto solid_task{TaskToSolidTask(task_factory.GetNextTask(), 1)};
  auto command = step_->execute(
      {{}, SolidTasks{std::move(solid_task)}, ModelController::Status::kOk});
  EXPECT_NE(dynamic_cast<CompleteTasksCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(CompleteReplStepTest,
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

TEST_F(CompleteReplStepTest,
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

TEST_F(CompleteReplStepTest,
       SecondCallTaskWithConfirmationNoMustReturnVoidCommand) {
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
