#include "ScenarioFramework.h"

#if 0

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
  EXPECT_EQ(*storage.Find(CreateTaskId(0))->second, expected_task);
}

TEST_F(EditActionTest, EmptyInputShouldLeavePreviousData) {
  auto [title, date, priority, progress] =
      task_stringed_data_producer_.GetData();

  auto storage = RunScenario({"add", title, date, priority, progress, "y",
                              "edit 0", "", "", "", "", "y", "q"});

  auto expected_task = CreateTask(title, *validator_->ParseTaskDate(date),
                                  *validator_->ParseTaskPriority(priority),
                                  *validator_->ParseTaskProgress(progress));

  EXPECT_EQ(*storage.Find(CreateTaskId(0))->second, expected_task);
}
#endif