#include "model/task_wrapper/TaskWrapper.h"

#include <gtest/gtest.h>

#include "ModelUtils.h"

class TaskStorageTest : public ::testing::Test {};

TEST_F(TaskStorageTest, SimpleGetter) {
  TaskFactory tf;
  auto task = tf.GetNextTask();
  const TaskWrapper tw{task};
  ASSERT_EQ(*tw, task);
  ASSERT_EQ(tw.GetTask(), task);
}

TEST_F(TaskStorageTest, SimpleSetter) {
  TaskFactory tf;
  auto task = tf.GetNextTask();
  TaskWrapper tw{task};
  auto new_task = tf.GetNextTask();
  tw.SetTask(new_task);
  ASSERT_EQ(*tw, new_task);
  ASSERT_EQ(tw.GetTask(), new_task);
}

TEST_F(TaskStorageTest, SimpleComplete) {
  TaskFactory tf;
  auto task = tf.GetNextTask();
  TaskWrapper tw{task};
  tw.Complete();
  ASSERT_EQ(tw->progress(), Task::kCompleted);
}

TEST_F(TaskStorageTest, CompleteOnNestedTasks) {
  TaskFactory tf;
  MockTaskIdProducer ip;
  auto task = tf.GetNextTask();
  TaskWrapper tw{task};
  auto nested_task = tf.GetNextTask();
  auto nested_id = ip.GetNextId();
  tw.Add(nested_id, nested_task);

  tw.Complete();
  ASSERT_EQ(tw.Find(nested_id)->second->progress(), Task::kCompleted);
}
