#include "interactor/state_machine/interactor_steps/AddStep.h"

#include "StepTest.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class AddStepTest : public StepTest {
 public:
  void SetUp() override { StepTest::SetUp(); }

 protected:
  void SetArg(std::string arg) {
    step_ = std::make_unique<AddStep>(validator_, io_facility_,
                                      small_step_factory_, std::move(arg));
  }
  std::unique_ptr<AddStep> step_;
};

TEST_F(AddStepTest, FirstCallWithEmptyArgumentedMustReturnVoidCommand) {
  SetArg("");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddStepTest,
       FirstCallWithOneArgumentedMustReturnGetSpecifiedTasksCommand) {
  SetArg("12");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<GetSpecifiedTasksCommand*>(command.get()), nullptr);
}

TEST_F(AddStepTest,
       FirstCallWithMultipleArgumentedMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("12 12");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddStepTest,
       FirstCallWithInvalidArgrumentedMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("die");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddStepTest, SecondCallWithoutArgumentMustReturnAddTaskCommand) {
  SetArg("");
  step_->execute({});
  SetInput({"dafs", "", "", "", "y"});
  auto command = step_->execute({});
  EXPECT_NE(dynamic_cast<AddTaskCommand*>(command.get()), nullptr);
}

TEST_F(AddStepTest, SecondCallWithArgumentMustReturnAddSubtaskTaskCommand) {
  SetArg("1");
  step_->execute({});
  SetInput({"dafs", "", "", "", "y"});
  TaskFactory task_factory;
  auto solid_task{TaskToSolidTask(task_factory.GetNextTask(), 1)};
  auto command = step_->execute(
      {{}, SolidTasks{solid_task}, ModelController::Status::kOk});
  EXPECT_NE(dynamic_cast<AddSubtaskCommand*>(command.get()), nullptr);
}

TEST_F(AddStepTest,
       SecondCallWithoutTasksInContextMustReturnVoidAndThenCauseDeath) {
  SetArg("3");
  step_->execute({});
  SetInput({"dafs", "", "", "", "y"});
  auto command = step_->execute({});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddStepTest,
       SecondCallWithoutSuatibleTaskInContextMustReturnVoidAndThenCauseDeath) {
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

TEST_F(AddStepTest,
       SecondCallTaskWithRandomConfirmationMustReturnVoidAndThenCauseDeath) {
  SetArg("");
  step_->execute({});
  SetInput({"dafs", "", "", "", "gdf"});
  auto command = step_->execute({});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddStepTest,
       SecondCallSubtaskWithRandomConfirmationMustReturnVoidAndThenCauseDeath) {
  SetArg("1");
  step_->execute({});
  SetInput({"dafs", "", "", "", "gdf"});
  TaskFactory task_factory;
  auto solid_task{TaskToSolidTask(task_factory.GetNextTask(), 1)};
  auto command = step_->execute(
      {{}, SolidTasks{solid_task}, ModelController::Status::kOk});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddStepTest,
       SecondCallTaskWithConfirmationNoMustReturnVoidAndThenCauseDeath) {
  SetArg("");
  step_->execute({});
  SetInput({"dafs", "", "", "", "n"});
  auto command = step_->execute({});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddStepTest,
       SecondCallSubtaskWithConfirmationNoMustReturnVoidAndThenCauseDeath) {
  SetArg("1");
  step_->execute({});
  SetInput({"dafs", "", "", "", "n"});
  TaskFactory task_factory;
  auto solid_task{TaskToSolidTask(task_factory.GetNextTask(), 1)};
  auto command = step_->execute(
      {{}, SolidTasks{solid_task}, ModelController::Status::kOk});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddStepTest, ThirdCallSubtaskMustReturnVoidCommand) {
  SetArg("1");
  step_->execute({});
  SetInput({"dafs", "", "", "", "y"});
  TaskFactory task_factory;
  auto solid_task{TaskToSolidTask(task_factory.GetNextTask(), 1)};
  step_->execute({{}, SolidTasks{solid_task}, ModelController::Status::kOk});
  auto command = step_->execute({CreateTaskId(0), {}, {}});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddStepTest, ThirdCallTaskMustReturnVoidCommand) {
  SetArg("");
  step_->execute({});
  SetInput({"dafs", "", "", "", "y"});
  step_->execute({});
  auto command = step_->execute({CreateTaskId(0), {}, {}});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddStepTest,
       ThirdCallSubtaskWithoutTaskIdInContextMustReturnVoidCommand) {
  SetArg("1");
  step_->execute({});
  SetInput({"dafs", "", "", "", "y"});
  TaskFactory task_factory;
  auto solid_task{TaskToSolidTask(task_factory.GetNextTask(), 1)};
  step_->execute({{}, SolidTasks{solid_task}, ModelController::Status::kOk});
  auto command = step_->execute({});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddStepTest, ThirdCallTaskWithoutTaskIdInContextMustReturnVoidCommand) {
  SetArg("");
  step_->execute({});
  SetInput({"dafs", "", "", "", "y"});
  step_->execute({});
  auto command = step_->execute({});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}
