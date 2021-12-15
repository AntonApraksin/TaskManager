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
  EXPECT_EQ(storage.Find(CreateTaskId(0))->progress(), Task::kCompleted);
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
  auto task_wrapper = storage.Find(CreateTaskId(0));
  auto subtask_wrapper = task_wrapper.Find(CreateTaskId(1));
  auto subsubtask_wrapper = subtask_wrapper.Find(CreateTaskId(2));
  EXPECT_EQ(task_wrapper->progress(), Task::kCompleted);
  EXPECT_EQ(subtask_wrapper->progress(), Task::kCompleted);
  EXPECT_EQ(subsubtask_wrapper->progress(), Task::kCompleted);
}
