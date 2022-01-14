#include "repl/state_machine/commands/Commands.h"
#include "repl/unit/state_machine/commands/CommandTest.h"

class VoidCommandTest : public CommandTest {};

TEST_F(VoidCommandTest, MustDoNothing) {
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

  auto old_dump = model_controller_->GetAllSolidTasks().AccessResult();

  VoidCommand command;
  auto ctx = command.execute(*model_controller_);
  auto new_dump = model_controller_->GetAllSolidTasks().AccessResult();
  ASSERT_EQ(ctx.status, ModelController::Status::kOk);
  ASSERT_EQ(old_dump.size(), new_dump.size());

  for (size_t i{0}, sz{old_dump.size()}; i != sz; ++i) {
    EXPECT_EQ(old_dump[i], new_dump[i]);
  }
}