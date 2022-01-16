#include "interactor/state_machine/commands/Commands.h"
#include "interactor/unit/state_machine/commands/CommandTest.h"

class EditTaskCommandTest : public CommandTest {};

TEST_F(EditTaskCommandTest, MustEditTask) {
  auto task = task_factory_.GetNextTask();
  auto edited_task = task_factory_.GetNextTask();
  model_controller_->Add(task);
  EditTaskCommand command{CreateTaskId(0), edited_task};
  auto ctx = command.execute(*model_controller_);
  ASSERT_EQ(ctx.status, ModelController::Status::kOk);
  auto dump = model_controller_->GetAllSolidTasks();
  EXPECT_EQ(dump.AccessResult()[0].task(), edited_task);
}

TEST_F(EditTaskCommandTest, MustSetStatusNotPresentId) {
  auto task = task_factory_.GetNextTask();
  auto edited_task = task_factory_.GetNextTask();
  model_controller_->Add(task);
  EditTaskCommand command{CreateTaskId(2), edited_task};
  auto ctx = command.execute(*model_controller_);
  ASSERT_EQ(ctx.status, ModelController::Status::kNotPresentId);
}
