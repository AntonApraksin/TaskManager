#include "interactor/state_machine/commands/Commands.h"
#include "interactor/unit/state_machine/commands/CommandTest.h"
#include "test_utils/operators.h"

class DeleteLabelCommandTest : public CommandTest {};

TEST_F(DeleteLabelCommandTest, MustDeleteLabel) {
  auto task = task_factory_.GetNextTask();
  auto id = model_controller_->Add(task).AccessResult();
  Label label;
  label.set_name("label1");
  model_controller_->AddLabel(id, label);
  DeleteLabelCommand delete_label_command{id, label};
  auto command_result = delete_label_command.execute(*model_controller_);
  ASSERT_EQ(command_result.status, ModelController::Status::kOk);
  auto storage = model_controller_->GetAllSolidTasks().AccessResult();
  auto got_task = storage.at(0).task();
  ASSERT_EQ(got_task.labels_size(), 0);
}