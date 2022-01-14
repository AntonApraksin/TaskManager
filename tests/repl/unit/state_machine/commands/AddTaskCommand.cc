#include "repl/state_machine/commands/Commands.h"
#include "repl/unit/state_machine/commands/CommandTest.h"

class AddTaskCommandTest : public CommandTest {};

TEST_F(AddTaskCommandTest, MustAddTask) {
  auto task = task_factory_.GetNextTask();
  AddTaskCommand command(task);
  auto ctx = command.execute(*model_controller_);
  ASSERT_EQ(ctx.status, ModelController::Status::kOk);
  EXPECT_EQ(ctx.task_id, CreateTaskId(0));
  EXPECT_EQ(model_controller_->GetSpecificSolidTasks({CreateTaskId(0)})
                .AccessResult()[0]
                .task(),
            task);
}
