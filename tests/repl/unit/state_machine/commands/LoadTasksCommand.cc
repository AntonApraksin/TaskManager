#include "repl/state_machine/commands/Commands.h"
#include "repl/unit/state_machine/commands/CommandTest.h"

class LoadTasksCommandTest : public CommandTest {};

TEST_F(LoadTasksCommandTest, MustLoad) {
  auto id_producer = std::make_unique<TaskIdProducer>();
  auto task_manager = std::make_unique<TaskManager>(std::move(id_producer));
  auto persistence = std::make_unique<Persistence>();
  ModelController new_model_controller(std::move(task_manager),
                                       std::move(persistence));

  auto _0 = task_factory_.GetNextTask();
  auto __1 = task_factory_.GetNextTask();
  auto __2 = task_factory_.GetNextTask();
  auto ___4 = task_factory_.GetNextTask();
  auto ___5 = task_factory_.GetNextTask();
  auto ____6 = task_factory_.GetNextTask();
  auto _3 = task_factory_.GetNextTask();
  new_model_controller.Add(_0);
  new_model_controller.Add(CreateTaskId(0), __1);
  new_model_controller.Add(CreateTaskId(0), __2);
  new_model_controller.Add(_3);
  new_model_controller.Add(CreateTaskId(2), ___4);
  new_model_controller.Add(CreateTaskId(2), ___5);
  new_model_controller.Add(CreateTaskId(5), ____6);
  auto file = std::make_unique<std::stringstream>();
  new_model_controller.SaveTo(*file);

  LoadTasksCommand command(std::move(file));
  auto ctx = command.execute(*model_controller_);
  ASSERT_EQ(ctx.status, ModelController::Status::kOk);

  auto dump1 = new_model_controller.GetAllSolidTasks().AccessResult();
  auto dump2 = model_controller_->GetAllSolidTasks().AccessResult();
  std::sort(dump1.begin(), dump1.end(), [](const auto& lhs, const auto& rhs) {
    return lhs.task_id() < rhs.task_id();
  });
  std::sort(dump2.begin(), dump2.end(), [](const auto& lhs, const auto& rhs) {
    return lhs.task_id() < rhs.task_id();
  });
  ASSERT_EQ(dump1.size(), dump2.size());
  for (size_t i{0}, sz{dump1.size()}; i != sz; ++i) {
    EXPECT_EQ(dump1[i], dump2[i]);
  }
}

TEST_F(LoadTasksCommandTest, MustReturnLoadFailureOnCorruptedData) {
  auto id_producer = std::make_unique<TaskIdProducer>();
  auto task_manager = std::make_unique<TaskManager>(std::move(id_producer));
  auto persistence = std::make_unique<Persistence>();
  ModelController new_model_controller(std::move(task_manager),
                                       std::move(persistence));

  auto _0 = task_factory_.GetNextTask();
  auto __1 = task_factory_.GetNextTask();
  auto __2 = task_factory_.GetNextTask();
  auto ___4 = task_factory_.GetNextTask();
  auto ___5 = task_factory_.GetNextTask();
  auto ____6 = task_factory_.GetNextTask();
  auto _3 = task_factory_.GetNextTask();
  new_model_controller.Add(_0);
  new_model_controller.Add(CreateTaskId(0), __1);
  new_model_controller.Add(CreateTaskId(0), __2);
  new_model_controller.Add(_3);
  new_model_controller.Add(CreateTaskId(2), ___4);
  new_model_controller.Add(CreateTaskId(2), ___5);
  new_model_controller.Add(CreateTaskId(5), ____6);
  auto file = std::make_unique<std::stringstream>();
  new_model_controller.SaveTo(*file);
  char ary[12];
  file->read(ary, 12);

  LoadTasksCommand command(std::move(file));
  auto ctx = command.execute(*model_controller_);
  ASSERT_EQ(ctx.status, ModelController::Status::kLoadFailure);
}
