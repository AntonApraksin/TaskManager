#include "repl/state_machine/repl_steps/ShowReplStep.h"

#include "ReplStepTest.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

class ShowReplStepTest : public ReplStepTest {
 public:
  void SetUp() override { ReplStepTest::SetUp(); }

 protected:
  void SetArg(std::string arg) {
    step_ = std::make_unique<ShowReplStep>(validator_, io_facility_,
                                           small_step_factory_, std::move(arg));
  }
  std::unique_ptr<ShowReplStep> step_;
};

TEST_F(ShowReplStepTest, FirstCallWithoutArgumentMustReturnGetAllTasksCommand) {
  SetArg("");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<GetAllTasksCommand*>(command.get()), nullptr);
}

TEST_F(ShowReplStepTest,
       FirstCallWithArgumentMustReturnGetSpecifiedTasksCommand) {
  SetArg("12 43");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<GetSpecifiedTasksCommand*>(command.get()), nullptr);
}

TEST_F(ShowReplStepTest,
       FirstCallWithInvalidIdArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("12 qw");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(ShowReplStepTest,
       FirstCallWithRepeatedIdArgumentMustReturnVoidCommandAndThenCauseDeath) {
  SetArg("12 34 12");
  auto command{step_->execute({})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(ShowReplStepTest, SecondCallWithNotPresentIdReturnVoidCommand) {
  SetArg("12 34");
  step_->execute({});
  auto command{
      step_->execute({{}, {}, ModelController::Status::kNotPresentId})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(ShowReplStepTest, SecondCallWithEmptySolidTasks) {
  SetArg("");
  step_->execute({});
  auto command{step_->execute({{}, {}, ModelController::Status::kOk})};
  EXPECT_NE(dynamic_cast<VoidCommand*>(command.get()), nullptr);
  EXPECT_DEATH(step_->execute({}), "");
}

TEST_F(ShowReplStepTest, SecondCallWithEverythingOkReturnVoidCommand) {
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