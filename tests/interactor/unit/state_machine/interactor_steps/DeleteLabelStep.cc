#include "interactor/state_machine/interactor_steps/DeleteLabelStep.h"

#include "StepTest.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class DeleteLabelStepTest : public StepTest {
 public:
  void SetUp() override { StepTest::SetUp(); }

 protected:
  void SetArg(std::string arg) {
    step_ = std::make_unique<DeleteLabelStep>(
        validator_, io_facility_, small_step_factory_, std::move(arg));
  }
  std::unique_ptr<DeleteLabelStep> step_;
};

TEST_F(DeleteLabelStepTest,
       FirstCallWithoutArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(DeleteLabelStepTest,
       FirstCallWithIdButWithoutLabelMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("12");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(DeleteLabelStepTest,
       FirstCallWithIdAndLabelMustReturnGetSpecifiedTasksCommand) {
  SetArg("12 foo");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<GetSpecifiedTasksCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(DeleteLabelStepTest, SecondCallWithoutSolidTasksReturnVoidCommand) {
  SetArg("12 foo");
  step_->execute({});
  auto command{
      step_->execute({{}, {}, ModelController::Status::kNotPresentId})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(DeleteLabelStepTest,
       SecondCallWithRandomConfirmationMustReturnVoidCommand) {
  SetArg("12 foo");
  step_->execute({});
  SetInput({"dfa"});
  TaskFactory task_factory;
  SolidTasks solid_tasks{TaskToSolidTask(task_factory.GetNextTask(), 12)};
  auto command =
      step_->execute({{}, solid_tasks, ModelController::Status::kOk});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(DeleteLabelStepTest, SecondCallWithConfirmationNoMustReturnVoidCommand) {
  SetArg("12 foo");
  step_->execute({});
  SetInput({"n"});
  TaskFactory task_factory;
  SolidTasks solid_tasks{TaskToSolidTask(task_factory.GetNextTask(), 12)};
  auto command =
      step_->execute({{}, solid_tasks, ModelController::Status::kOk});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(DeleteLabelStepTest,
       SecondCallWithConfirmationYesMustReturnDeleteLabelCommand) {
  SetArg("12 foo");
  step_->execute({});
  SetInput({"y"});
  TaskFactory task_factory;
  auto task = task_factory.GetNextTask();
  task.add_labels()->set_name("foo");
  SolidTasks solid_tasks{TaskToSolidTask(task, 12)};
  auto command =
      step_->execute({{}, solid_tasks, ModelController::Status::kOk});
  EXPECT_NE(dynamic_cast<DeleteLabelCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}
