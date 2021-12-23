#include <gtest/gtest.h>

#include "../ModelUtils.h"

class TaskTest : public ::testing::Test {};

TEST_F(TaskTest, FieldsAreConsistent) {
  std::string task_title = "Test task title";
  TaskDate_t task_due_date = parse_date("03/11/2020");
  Task::Priority task_priority = Task::kMedium;
  Task::Progress task_progress = Task::kUncompleted;

  const auto task =
      *CreateTask(task_title, task_due_date, task_priority, task_progress);

  EXPECT_EQ(task_title, task.title());
  EXPECT_EQ(task_due_date, task.due_date());
  EXPECT_EQ(task_priority, task.priority());
  EXPECT_EQ(task_progress, task.progress());
}

TEST_F(TaskTest, EmptyOptionalOnEmptyTitle) {
  std::string task_title = "";
  TaskDate_t task_due_date = parse_date("03/11/2020");
  Task::Priority task_priority = Task::kMedium;
  Task::Progress task_progress = Task::kCompleted;

  // NOLINTNEXTLINE
  EXPECT_FALSE(
      CreateTask(task_title, task_due_date, task_priority, task_progress));
}
