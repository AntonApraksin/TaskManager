#include "model/task/Task.h"

#include <gtest/gtest.h>

class TaskTest : public ::testing::Test {};

TEST_F(TaskTest, FieldsAreConsistent) {
  std::string task_title = "Test task title";
  Date_t task_due_date = parse_date("03/11/2020");
  Task::Priority task_priority = Task::Priority::kMedium;
  Task::State task_state = Task::State::kInProgress;

  const auto task =
      *Task::Create(task_title, task_priority, task_due_date, task_state);

  EXPECT_EQ(task_title, task.GetTitle());
  EXPECT_EQ(task_due_date, task.GetDueDate());
  EXPECT_EQ(task_priority, task.GetPriority());
  EXPECT_EQ(task_state, task.GetState());
}

TEST_F(TaskTest, EmptyOptionalOnEmptyTitle) {
  std::string task_title = "";
  Date_t task_due_date = parse_date("03/11/2020");
  Task::Priority task_priority = Task::Priority::kMedium;

  // NOLINTNEXTLINE
  EXPECT_FALSE(Task::Create(task_title, task_priority, task_due_date));
}
