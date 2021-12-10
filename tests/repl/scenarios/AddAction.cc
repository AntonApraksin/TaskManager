#include "Framework.h"

class AddActionTest : public testing::Test, protected Framework {
 protected:
  void SetUp() override { return SetUpImpl(); }
};

TEST_F(AddActionTest, OneTaskShouldBeAdded) {
  auto [title, date, priority, state] = task_stringed_data_producer_.GetData();

  auto storage = RunScenario({"add", title, date, priority, state, "y", "q"});

  auto expected_task = Task::Create(
      title, *validator_->ParseTaskPriority(priority),
      *validator_->ParseTaskDate(date), *validator_->ParseTaskState(state));

  EXPECT_EQ(*storage.Find(TaskId::Create(0)), expected_task);
}

TEST_F(AddActionTest, NTasksShouldBeAdded) {
  constexpr int kNTasks = 64;
  std::vector<TaskStringedData> tasks_data;
  for (int i{0}; i != kNTasks; ++i) {
    tasks_data.push_back(task_stringed_data_producer_.GetData());
  }

  std::vector<std::string> commands;
  std::for_each(tasks_data.cbegin(), tasks_data.cend(),
                [&commands](const auto& command) {
                  commands.push_back("add");
                  commands.push_back(command.title);
                  commands.push_back(command.date);
                  commands.push_back(command.priority);
                  commands.push_back(command.state);
                  commands.push_back("y");
                });
  commands.push_back("q");

  auto storage = RunScenario(std::move(commands));

  for (int i{0}; i != kNTasks; ++i) {
    EXPECT_EQ(*storage.Find(TaskId::Create(i)),
              TaskDataToTask(tasks_data.at(i)));
  }
}

TEST_F(AddActionTest, NestedTasksShouldBeAdded) {
  auto task = task_stringed_data_producer_.GetData();
  auto subtask = task_stringed_data_producer_.GetData();
  auto subsubtask = task_stringed_data_producer_.GetData();

  std::vector<std::string> commands;
  auto storage = RunScenario({"add", task.title, task.date, task.priority,
                              task.state, "y", "add 0", subtask.title,
                              subtask.date, subtask.priority, subtask.state,
                              "y", "add 1", subsubtask.title, subsubtask.date,
                              subsubtask.priority, subsubtask.state, "y", "q"});

  auto task_wrapper = storage.Find(TaskId::Create(0));
  auto subtask_wrapper = task_wrapper.Find(TaskId::Create(1));
  auto subsubtask_wrapper = subtask_wrapper.Find(TaskId::Create(2));

  EXPECT_EQ(*task_wrapper, TaskDataToTask(task));
  EXPECT_EQ(*subtask_wrapper, TaskDataToTask(subtask));
  EXPECT_EQ(*subsubtask_wrapper, TaskDataToTask(subsubtask));
}

TEST_F(AddActionTest, NestedTaskShouldInheritParentsData) {
  std::string title = "title";
  std::string date = "12:34 21/07/2021";
  std::string priority = "medium";
  std::string state = "+";

  auto storage = RunScenario({"add", title, date, priority, state, "y", "add 0",
                              "subtitle", "", "", "", "y", "q"});

  EXPECT_EQ(storage.Find(TaskId::Create(0))->GetDueDate(),
            storage.Find(TaskId::Create(1))->GetDueDate());
  EXPECT_EQ(storage.Find(TaskId::Create(0))->GetPriority(),
            storage.Find(TaskId::Create(1))->GetPriority());
  EXPECT_EQ(storage.Find(TaskId::Create(0))->GetState(),
            storage.Find(TaskId::Create(1))->GetState());
}