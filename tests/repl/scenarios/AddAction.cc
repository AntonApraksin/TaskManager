#include "ScenarioFramework.h"

class AddActionTest : public testing::Test, protected ScenarioFramework {
 protected:
  void SetUp() override { return SetUpImpl(); }
};

TEST_F(AddActionTest, OneTaskShouldBeAdded) {
  auto [title, date, priority, progress] =
      task_stringed_data_producer_.GetData();

  auto storage =
      RunScenario({"add", title, date, priority, progress, "y", "q"});

  auto expected_task = CreateTask(title, *validator_->ParseTaskDate(date),
                                  *validator_->ParseTaskPriority(priority),
                                  *validator_->ParseTaskProgress(progress));

  EXPECT_EQ(*storage.Find(CreateTaskId(0))->second, expected_task);
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
    EXPECT_EQ(*storage.Find(CreateTaskId(i))->second,
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

  auto task_wrapper = storage.Find(CreateTaskId(0))->second;
  auto subtask_wrapper = task_wrapper.Find(CreateTaskId(1))->second;
  auto subsubtask_wrapper = subtask_wrapper.Find(CreateTaskId(2))->second;

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

  EXPECT_EQ(storage.Find(CreateTaskId(0))->second->due_date(),
            storage.Find(CreateTaskId(1))->second->due_date());
  EXPECT_EQ(storage.Find(CreateTaskId(0))->second->priority(),
            storage.Find(CreateTaskId(1))->second->priority());
  EXPECT_EQ(storage.Find(CreateTaskId(0))->second->progress(),
            storage.Find(CreateTaskId(1))->second->progress());
}