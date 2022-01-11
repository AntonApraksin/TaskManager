#include "ReplStepTest.h"
#include "repl/state_machine/repl_steps/AddReplStep.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class AddReplStepTest : public ReplStepTest {
 public:
  void SetUp() override { ReplStepTest::SetUp(); }

 protected:
  void SetArg(std::string arg) {
    step_ = std::make_unique<AddReplStep>(validator_, io_facility_,
                                          small_step_factory_, std::move(arg));
  }
  std::unique_ptr<AddReplStep> step_;
};

TEST_F(AddReplStepTest, FirstCallWithEmptyArgumentedMustReturnVoidCommand) {
  SetArg("");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddReplStepTest,
       FirstCallWithOneArgumentedMustReturnGetSpecifiedTasksCommand) {
  SetArg("12");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<GetSpecifiedTasksCommand*>(command.get()), nullptr);
}

TEST_F(AddReplStepTest,
       FirstCallWithMultipleArgumentedMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("12 12");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddReplStepTest,
       FirstCallWithInvalidArgrumentedMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("die");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddReplStepTest, SecondCallWithoutArgumentMustReturnAddTaskCommand) {
  SetArg("");
  step_->execute({});
  SetInput({"dafs", "", "", "", "y"});
  auto command = step_->execute({});
  EXPECT_NE(dynamic_cast<AddTaskCommand*>(command.get()), nullptr);
}

TEST_F(AddReplStepTest, SecondCallWithArgumentMustReturnAddSubtaskTaskCommand) {
  SetArg("1");
  step_->execute({});
  SetInput({"dafs", "", "", "", "y"});
  TaskFactory task_factory;
  auto solid_task{TaskToSolidTask(task_factory.GetNextTask(), 1)};
  auto command = step_->execute(
      {{}, SolidTasks{solid_task}, ModelController::Status::kOk});
  EXPECT_NE(dynamic_cast<AddSubtaskCommand*>(command.get()), nullptr);
}

TEST_F(AddReplStepTest,
       SecondCallWithoutTasksInContextMustReturnVoidAndThenCauseDeath) {
  SetArg("3");
  step_->execute({});
  SetInput({"dafs", "", "", "", "y"});
  auto command = step_->execute({});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddReplStepTest,
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

TEST_F(AddReplStepTest,
       SecondCallTaskWithRandomConfirmationMustReturnVoidAndThenCauseDeath) {
  SetArg("");
  step_->execute({});
  SetInput({"dafs", "", "", "", "gdf"});
  auto command = step_->execute({});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddReplStepTest,
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

TEST_F(AddReplStepTest,
       SecondCallTaskWithConfirmationNoMustReturnVoidAndThenCauseDeath) {
  SetArg("");
  step_->execute({});
  SetInput({"dafs", "", "", "", "n"});
  auto command = step_->execute({});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(AddReplStepTest,
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

TEST_F(AddReplStepTest, ThirdCallSubtaskMustReturnVoidCommand) {
  SetArg("1");
  step_->execute({});
  SetInput({"dafs", "", "", "", "y"});
  TaskFactory task_factory;
  auto solid_task{TaskToSolidTask(task_factory.GetNextTask(), 1)};
  step_->execute({{}, SolidTasks{solid_task}, ModelController::Status::kOk});
  auto command = step_->execute({CreateTaskId(0), {}, {}});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddReplStepTest, ThirdCallTaskMustReturnVoidCommand) {
  SetArg("");
  step_->execute({});
  SetInput({"dafs", "", "", "", "y"});
  step_->execute({});
  auto command = step_->execute({CreateTaskId(0), {}, {}});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}

TEST_F(AddReplStepTest,
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

TEST_F(AddReplStepTest,
       ThirdCallTaskWithoutTaskIdInContextMustReturnVoidCommand) {
  SetArg("");
  step_->execute({});
  SetInput({"dafs", "", "", "", "y"});
  step_->execute({});
  auto command = step_->execute({});
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
}
