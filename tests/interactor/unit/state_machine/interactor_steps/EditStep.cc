#include "interactor/state_machine/interactor_steps/EditStep.h"

#include "StepTest.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class EditStepTest : public StepTest {
 public:
  void SetUp() override { StepTest::SetUp(); }

 protected:
  void SetArg(std::string arg) {
    step_ = std::make_unique<EditStep>(validator_, io_facility_,
                                       small_step_factory_, std::move(arg));
  }
  std::unique_ptr<EditStep> step_;
};

TEST_F(EditStepTest,
       FirstCallWithoutArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(EditStepTest,
       FirstCallWithMultipleArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("12 43");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(EditStepTest,
       FirstCallWithInvalidIdArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("qw");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(EditStepTest, FirstCallWithArgumentMustReturnGetSpecifiedTasksCommand) {
  SetArg("12");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<GetSpecifiedTasksCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(EditStepTest, SecondCallWithoutSolidTasksReturnVoidCommand) {
  SetArg("12");
  step_->execute({});
  auto command{
      step_->execute({{}, {}, ModelController::Status::kNotPresentId})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(EditStepTest, SecondCallWithoutProperTaskReturnVoidCommand) {
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

TEST_F(EditStepTest,
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

TEST_F(EditStepTest, SecondCallWithRandomConfirmationMustReturnVoidCommand) {
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

TEST_F(EditStepTest, SecondCallWithConfirmationNoMustReturnVoidCommand) {
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
