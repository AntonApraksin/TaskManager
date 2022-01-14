#include "repl/state_machine/repl_steps/EditReplStep.h"

#include "ReplStepTest.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class EditReplStepTest : public ReplStepTest {
 public:
  void SetUp() override { ReplStepTest::SetUp(); }

 protected:
  void SetArg(std::string arg) {
    step_ = std::make_unique<EditReplStep>(validator_, io_facility_,
                                           small_step_factory_, std::move(arg));
  }
  std::unique_ptr<EditReplStep> step_;
};

TEST_F(EditReplStepTest,
       FirstCallWithoutArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(EditReplStepTest,
       FirstCallWithMultipleArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("12 43");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(EditReplStepTest,
       FirstCallWithInvalidIdArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("qw");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(EditReplStepTest,
       FirstCallWithArgumentMustReturnGetSpecifiedTasksCommand) {
  SetArg("12");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<GetSpecifiedTasksCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(EditReplStepTest, SecondCallWithoutSolidTasksReturnVoidCommand) {
  SetArg("12");
  step_->execute({});
  auto command{
      step_->execute({{}, {}, ModelController::Status::kNotPresentId})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(EditReplStepTest, SecondCallWithoutProperTaskReturnVoidCommand) {
  SetArg("3");
  step_->execute({});
  SetInput({"dafs", "", "", "", "y"});
  TaskFactory task_factory;
  SolidTasks solid_tasks{TaskToSolidTask(task_factory.GetNextTask(), 1),
                         TaskToSolidTask(task_factory.GetNextTask(), 2),
                         TaskToSolidTask(task_factory.GetNextTask(), 4, 2)};
  auto command =
      step_->execute({{}, solid_tasks, ModelController::Status::kOk});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(EditReplStepTest,
       SecondCallWithProperTaskAndConfirmationYesMustReturnEditCommand) {
  SetArg("3");
  step_->execute({});
  SetInput({"dafs", "", "", "", "y"});
  TaskFactory task_factory;
  SolidTasks solid_tasks{TaskToSolidTask(task_factory.GetNextTask(), 3)};
  auto command =
      step_->execute({{}, solid_tasks, ModelController::Status::kOk});
  EXPECT_NE(dynamic_cast<EditTaskCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(EditReplStepTest,
       SecondCallWithRandomConfirmationMustReturnVoidCommand) {
  SetArg("3");
  step_->execute({});
  SetInput({"dafs", "", "", "", "dfa"});
  TaskFactory task_factory;
  SolidTasks solid_tasks{TaskToSolidTask(task_factory.GetNextTask(), 3)};
  auto command =
      step_->execute({{}, solid_tasks, ModelController::Status::kOk});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(EditReplStepTest, SecondCallWithConfirmationNoMustReturnVoidCommand) {
  SetArg("3");
  step_->execute({});
  SetInput({"dafs", "", "", "", "n"});
  TaskFactory task_factory;
  SolidTasks solid_tasks{TaskToSolidTask(task_factory.GetNextTask(), 3)};
  auto command =
      step_->execute({{}, solid_tasks, ModelController::Status::kOk});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}
