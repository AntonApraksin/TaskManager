#include <gtest/gtest.h>

#include "test_utils/ScenarioFramework.h"
#include "test_utils/operators.h"
#include "test_utils/utils.h"

class EditActionTest : public testing::Test, protected ScenarioFramework {
 protected:
  void SetUp() override { return SetUpImpl(); }
};

TEST_F(EditActionTest, OneTaskShouldBeEdited) {
  auto initial = task_stringed_data_producer_.GetData();
  auto edited = task_stringed_data_producer_.GetData();

  auto [storage, _] =
      RunScenario({"add", initial.title, initial.date, initial.priority,
                   initial.state, "y", "edit 0", edited.title, edited.date,
                   edited.priority, edited.state, "y", "q"});

  auto expected_task = TaskDataToSolidTask(edited, 0);
  EXPECT_EQ(FindSolidTask(storage, 0), expected_task);
}

TEST_F(EditActionTest, EmptyInputShouldLeavePreviousData) {
  auto t = task_stringed_data_producer_.GetData();

  auto [storage, _] = RunScenario({"add", t.title, t.date, t.priority, t.state,
                                   "y", "edit 0", "", "", "", "", "y", "q"});

  auto expected_task = TaskDataToSolidTask(t, 0);

  EXPECT_EQ(FindSolidTask(storage, 0), expected_task);
}