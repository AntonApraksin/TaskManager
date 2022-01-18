#include "interactor/state_machine/interactor_steps/AddLabelStep.h"

#include "StepTest.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class AddLabelStepTest : public StepTest {
 public:
  void SetUp() override { StepTest::SetUp(); }

 protected:
  void SetArg(std::string arg) {
    step_ = std::make_unique<AddLabelStep>(validator_, io_facility_,
                                           small_step_factory_, std::move(arg));
  }
  std::unique_ptr<AddLabelStep> step_;
};

TEST_F(AddLabelStepTest,
       FirstCallWithoutArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddLabelStepTest,
       FirstCallWithIdButWithoutLabelMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("12");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddLabelStepTest,
       FirstCallWithIdAndLabelMustReturnGetSpecifiedTasksCommand) {
  SetArg("12 foo");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<GetSpecifiedTasksCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddLabelStepTest, FirstCallWithInvalidIdAndLabelMustReturnVoidCommand) {
  SetArg("a foo");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddLabelStepTest, FirstCallWithThirdArgumentMustReturnVoidCommand) {
  SetArg("12 foo 34");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddLabelStepTest, SecondCallWithoutSolidTasksReturnVoidCommand) {
  SetArg("12 foo");
  step_->execute({});
  auto command{
      step_->execute({{}, {}, ModelController::Status::kNotPresentId})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddLabelStepTest,
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

TEST_F(AddLabelStepTest, SecondCallWithConfirmationNoMustReturnVoidCommand) {
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

TEST_F(AddLabelStepTest,
       SecondCallWithConfirmationYesMustReturnAddLabelCommand) {
  SetArg("12 foo");
  step_->execute({});
  SetInput({"y"});
  TaskFactory task_factory;
  SolidTasks solid_tasks{TaskToSolidTask(task_factory.GetNextTask(), 12)};
  auto command =
      step_->execute({{}, solid_tasks, ModelController::Status::kOk});
  EXPECT_NE(dynamic_cast<AddLabelCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddLabelStepTest, MustNotChangeStepAfterFirstExecute) {
  SetArg("12 foo");
  step_->execute({});
  std::shared_ptr<Step> to_change{std::make_shared<StepChangeStepTesting>()};
  const auto old_addr = to_change.get();
  step_->ChangeStep(to_change);
  EXPECT_EQ(to_change.get(), old_addr);
}

TEST_F(AddLabelStepTest, MustChangeStepAfterSecondExecute) {
  SetArg("12 foo");
  step_->execute({});
  SetInput({"y"});
  TaskFactory task_factory;
  auto task = task_factory.GetNextTask();
  SolidTasks solid_tasks{TaskToSolidTask(task, 12)};
  step_->execute({{}, solid_tasks, ModelController::Status::kOk});
  std::shared_ptr<Step> to_change{std::make_shared<StepChangeStepTesting>()};
  const auto old_addr = to_change.get();
  step_->ChangeStep(to_change);
  EXPECT_NE(to_change.get(), old_addr);
}
