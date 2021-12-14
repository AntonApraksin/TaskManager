#include "ScenarioFramework.h"

class EditActionTest : public testing::Test, protected ScenarioFramework {
 protected:
  void SetUp() override { return SetUpImpl(); }
};

TEST_F(EditActionTest, OneTaskShouldBeEdited) {
  auto initial = task_stringed_data_producer_.GetData();
  auto edited = task_stringed_data_producer_.GetData();

  auto storage =
      RunScenario({"add", initial.title, initial.date, initial.priority,
                   initial.state, "y", "edit 0", edited.title, edited.date,
                   edited.priority, edited.state, "y", "q"});

  auto expected_task = TaskDataToTask(edited);
  EXPECT_EQ(*storage.Find(TaskId::Create(0)), expected_task);
}

TEST_F(EditActionTest, EmptyInputShouldLeavePreviousData) {
  auto [title, date, priority, state] = task_stringed_data_producer_.GetData();

  auto storage = RunScenario({"add", title, date, priority, state, "y",
                              "edit 0", "", "", "", "", "y", "q"});

  auto expected_task = Task::Create(
      title, *validator_->ParseTaskPriority(priority),
      *validator_->ParseTaskDate(date), *validator_->ParseTaskState(state));

  EXPECT_EQ(*storage.Find(TaskId::Create(0)), expected_task);
}