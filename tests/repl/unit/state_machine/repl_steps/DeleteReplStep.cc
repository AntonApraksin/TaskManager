#include "repl/state_machine/repl_steps/DeleteReplStep.h"

#include "ReplStepTest.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class DeleteReplStepTest : public ReplStepTest {
 public:
  void SetUp() override { ReplStepTest::SetUp(); }

 protected:
  void SetArg(std::string arg) {
    step_ = std::make_unique<DeleteReplStep>(
        validator_, io_facility_, small_step_factory_, std::move(arg));
  }
  std::unique_ptr<DeleteReplStep> step_;
};

TEST_F(DeleteReplStepTest,
       FirstCallWithoutArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(DeleteReplStepTest,
       FirstCallWithArgumentMustReturnGetSpecifiedTasksCommand) {
  SetArg("12 43");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<GetSpecifiedTasksCommand*>(command.get()), nullptr);
}

TEST_F(DeleteReplStepTest,
       FirstCallWithInvalidIdArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("12 qw");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(DeleteReplStepTest,
       FirstCallWithRepeatedIdArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("12 34 12");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(DeleteReplStepTest, SecondCallWithoutSolidTasksReturnVoidCommand) {
  SetArg("12 34");
  step_->execute({});
  auto command{
      step_->execute({{}, {}, ModelController::Status::kNotPresentId})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(DeleteReplStepTest,
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

TEST_F(DeleteReplStepTest,
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

TEST_F(DeleteReplStepTest,
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

TEST_F(DeleteReplStepTest,
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
