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

  EXPECT_EQ(FindSolidTask(storage, 0).task(), expected_task);
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
    EXPECT_EQ(FindSolidTask(storage, i),
              TaskDataToSolidTask(tasks_data.at(i), i));
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
  auto top_task = FindSolidTask(storage, 0);
  auto sub_top_task = FindSolidTask(storage, 1);
  auto sub_sub_top_task = FindSolidTask(storage, 2);
  EXPECT_EQ(top_task, TaskDataToSolidTask(task, 0));
  EXPECT_EQ(sub_top_task, TaskDataToSolidTask(subtask, 1, 0));
  EXPECT_EQ(sub_sub_top_task, TaskDataToSolidTask(subsubtask, 2, 1));
}

TEST_F(AddActionTest, NestedTaskShouldInheritParentsData) {
  std::string title = "title";
  std::string date = "12:34 21/07/2021";
  std::string priority = "medium";
  std::string state = "+";

  auto storage = RunScenario({"add", title, date, priority, state, "y", "add 0",
                              "subtitle", "", "", "", "y", "q"});
  auto t1 = FindSolidTask(storage, 0);
  auto t2 = FindSolidTask(storage, 1);

  EXPECT_EQ(t1.task().due_date(), t2.task().due_date());
  EXPECT_EQ(t1.task().priority(), t2.task().priority());
  EXPECT_EQ(t1.task().progress(), t2.task().progress());
}