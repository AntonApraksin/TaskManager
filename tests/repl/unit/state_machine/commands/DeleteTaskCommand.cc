#include "repl/state_machine/commands/Commands.h"
#include "repl/unit/state_machine/commands/CommandTest.h"

class DeleteTaskCommandTest : public CommandTest {};

TEST_F(DeleteTaskCommandTest, MustDeleteNTasks) {
  auto _0 = task_factory_.GetNextTask();
  auto __1 = task_factory_.GetNextTask();
  auto __2 = task_factory_.GetNextTask();
  auto ___4 = task_factory_.GetNextTask();
  auto ___5 = task_factory_.GetNextTask();
  auto ____6 = task_factory_.GetNextTask();
  auto _3 = task_factory_.GetNextTask();
  model_controller_->Add(_0);
  model_controller_->Add(CreateTaskId(0), __1);
  model_controller_->Add(CreateTaskId(0), __2);
  model_controller_->Add(_3);
  model_controller_->Add(CreateTaskId(2), ___4);
  model_controller_->Add(CreateTaskId(2), ___5);
  model_controller_->Add(CreateTaskId(5), ____6);

  DeleteTasksCommand command{{CreateTaskId(2), CreateTaskId(3)}};
  auto ctx = command.execute(*model_controller_);
  ASSERT_EQ(ctx.status, ModelController::Status::kOk);
  auto dump = model_controller_->GetAllSolidTasks().AccessResult();
  EXPECT_EQ(dump.size(), 2);
  EXPECT_EQ(dump[0].task(), _0);
  EXPECT_EQ(dump[1].task(), __1);
}

TEST_F(DeleteTaskCommandTest, MustSetStatusNotPresentId) {
  auto _0 = task_factory_.GetNextTask();
  model_controller_->Add(_0);
  DeleteTasksCommand command({CreateTaskId(0), CreateTaskId(2)});
  auto ctx = command.execute(*model_controller_);
  ASSERT_EQ(ctx.status, ModelController::Status::kNotPresentId);
}
