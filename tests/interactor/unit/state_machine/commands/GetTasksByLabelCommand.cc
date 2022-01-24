#include "interactor/state_machine/commands/Commands.h"
#include "interactor/unit/state_machine/commands/CommandTest.h"

class GetTasksByLabelCommandTest : public CommandTest {};

TEST_F(GetTasksByLabelCommandTest, MustReturnAllTasks) {
  auto task1 = task_factory_.GetNextTask();
  task1.add_labels()->set_name("label1");
  task1.add_labels()->set_name("label2");

  auto task2 = task_factory_.GetNextTask();
  task2.add_labels()->set_name("label1");

  auto task3 = task_factory_.GetNextTask();
  task2.add_labels()->set_name("label2");
  model_controller_->Add(task1);
  model_controller_->Add(task2);
  model_controller_->Add(task3);

  Label label;
  label.set_name("label1");
  GetTasksByLabelCommand command{std::move(label)};
  auto command_result = command.execute(*model_controller_);
  ASSERT_EQ(command_result.status, ModelController::Status::kOk);
  auto& storage = command_result.solid_tasks.value();
  EXPECT_EQ(storage.size(), 2);

  EXPECT_NE(std::find_if(storage.cbegin(), storage.cend(),
                         [&task1](auto solid_task) {
                           return solid_task.task() == task1;
                         }),
            storage.cend());
  EXPECT_NE(std::find_if(storage.cbegin(), storage.cend(),
                         [&task2](auto solid_task) {
                           return solid_task.task() == task2;
                         }),
            storage.cend());
}
