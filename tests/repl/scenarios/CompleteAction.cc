#include "ScenarioFramework.h"

class CompleteActionTest : public testing::Test, protected ScenarioFramework {
 protected:
  void SetUp() override { return SetUpImpl(); }
};

TEST_F(CompleteActionTest, TaskShouldBeCompleted) {
  std::string title = "title";
  std::string date = "12:34 21/07/2021";
  std::string priority = "medium";
  std::string state = "-";

  auto storage = RunScenario(
      {"add", title, date, priority, state, "y", "complete 0", "y", "q"});
  EXPECT_EQ(FindSolidTask(storage, 0).task().progress(), Task::kCompleted);
}

TEST_F(CompleteActionTest, NestedTasksShouldBeCompleted) {
  auto task = task_stringed_data_producer_.GetData();
  auto subtask = task_stringed_data_producer_.GetData();
  auto subsubtask = task_stringed_data_producer_.GetData();

  std::vector<std::string> commands;
  auto storage = RunScenario({"add",
                              task.title,
                              task.date,
                              task.priority,
                              "-",
                              "y",
                              "add 0",
                              subtask.title,
                              subtask.date,
                              subtask.priority,
                              "-",
                              "y",
                              "add 1",
                              subsubtask.title,
                              subsubtask.date,
                              subsubtask.priority,
                              "-",
                              "y",
                              "complete 0",
                              "y",
                              "q"});
  auto top_task = FindSolidTask(storage, 0);
  auto sub_top_task = FindSolidTask(storage, 1);
  auto sub_sub_top_task = FindSolidTask(storage, 2);
  EXPECT_EQ(top_task.task().progress(), Task::kCompleted);
  EXPECT_EQ(sub_top_task.task().progress(), Task::kCompleted);
  EXPECT_EQ(sub_sub_top_task.task().progress(), Task::kCompleted);
}