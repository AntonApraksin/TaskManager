#include "repl/state_machine/commands/Commands.h"
#include "repl/unit/state_machine/commands/CommandTest.h"
#include "test_utils/operators.h"

class AddSubtaskCommandTest : public CommandTest {};

TEST_F(AddSubtaskCommandTest, MustAddSubtask) {
  auto parent_task = task_factory_.GetNextTask();
  auto child_task = task_factory_.GetNextTask();
  model_controller_->Add(parent_task);
  AddSubtaskCommand command(CreateTaskId(0), child_task);
  auto ctx = command.execute(*model_controller_);
  ASSERT_EQ(ctx.status, ModelController::Status::kOk);
  EXPECT_EQ(ctx.task_id, CreateTaskId(1));
  EXPECT_EQ(model_controller_->GetSpecificSolidTasks({CreateTaskId(1)})
                .AccessResult()[0]
                .task(),
            child_task);
}

TEST_F(AddSubtaskCommandTest, MustSetStatusNotPresentId) {
  auto child_task = task_factory_.GetNextTask();
  auto command = AddSubtaskCommand(CreateTaskId(15), child_task);
  auto ctx = command.execute(*model_controller_);
  ASSERT_EQ(ctx.status, ModelController::Status::kNotPresentId);
}
