#include <gtest/gtest.h>

#include "ModelUtils.h"
#include "model/id/ITaskIdProducer.h"
#include "model/task_wrapper/TaskWrapper.h"

class TaskStorageTest : public ::testing::Test {};

TEST_F(TaskStorageTest, SimpleAdd) {
  TaskStorage ts;
  TaskFactory tf;
  MockTaskIdProducer ip;

  auto task = tf.GetNextTask();
  auto id = ip.GetNextId();
  ts.Add(id, task);
  auto got = *ts.Find(id);

  EXPECT_EQ(task, got);
}

TEST_F(TaskStorageTest, SimpleDelete) {
  TaskStorage ts;
  TaskFactory tf;
  MockTaskIdProducer ip;

  auto task = tf.GetNextTask();
  auto id = ip.GetNextId();

  ts.Add(id, task);

  auto& parent_of_deletable = ts.FindParentOf(id);
  parent_of_deletable.Delete(id);
  ASSERT_THROW(ts.Find(id), std::runtime_error);
}

TEST_F(TaskStorageTest, FindOnFindParentOfShouldWork) {
  TaskStorage ts;
  TaskFactory tf;
  MockTaskIdProducer ip;

  auto task = tf.GetNextTask();
  auto id = ip.GetNextId();

  auto nested_task = tf.GetNextTask();
  auto nested_id = ip.GetNextId();

  ts.Add(id, task);
  ts.Find(id).Add(nested_id, nested_task);

  ASSERT_EQ(*ts.FindParentOf(nested_id).Find(nested_id), nested_task);
}

TEST_F(TaskStorageTest, FindOnUnexistingIdShouldThrowRuntimeError) {
  TaskStorage ts;
  MockTaskIdProducer ip;

  auto id = ip.GetNextId();

  ASSERT_THROW(ts.Find(id), std::runtime_error);
}

TEST_F(TaskStorageTest, FindParentOfUnexistingIdShouldThrowRuntimeError) {
  TaskStorage ts;
  MockTaskIdProducer ip;

  auto id = ip.GetNextId();

  ASSERT_THROW(ts.FindParentOf(id), std::runtime_error);
}

TEST_F(TaskStorageTest, DeleteUnexistingIdShouldThrowRuntimeError) {
  TaskStorage ts;
  MockTaskIdProducer ip;

  auto id = ip.GetNextId();

  ASSERT_THROW(ts.Delete(id), std::runtime_error);
}

TEST_F(TaskStorageTest, FindShouldWorkOnConstantObjects) {
  TaskStorage ts;
  TaskFactory tf;
  MockTaskIdProducer ip;

  auto id = ip.GetNextId();
  auto task = tf.GetNextTask();

  ts.Add(id, task);
  auto const constant_ts = ts;

  ASSERT_EQ(*constant_ts.Find(id), task);
}

TEST_F(TaskStorageTest, FindParentOfShouldWorkOnConstantObjects) {
  TaskStorage ts;
  TaskFactory tf;
  MockTaskIdProducer ip;

  auto id = ip.GetNextId();
  auto task = tf.GetNextTask();

  ts.Add(id, task);
  auto const constant_ts = ts;

  ASSERT_EQ(*constant_ts.FindParentOf(id).Find(id), task);
}

TEST_F(TaskStorageTest,
       ConstantObjectShouldAllowReadAccessAcrossAllTaskLayers) {
  TaskStorage ts;
  TaskFactory tf;
  MockTaskIdProducer ip;

  auto id = ip.GetNextId();
  auto task = tf.GetNextTask();

  auto nested_id = ip.GetNextId();
  auto nested_task = tf.GetNextTask();

  ts.Add(id, task);
  ts.Find(id).Add(nested_id, nested_task);

  auto const constant_ts = ts;
  ASSERT_EQ(*constant_ts.FindParentOf(nested_id).Find(nested_id), nested_task);
  ASSERT_EQ(*constant_ts.Find(nested_id), nested_task);
}

TEST_F(TaskStorageTest, SimpleNestedAdd) {
  TaskStorage ts;
  TaskFactory tf;
  MockTaskIdProducer ip;

  auto task = tf.GetNextTask();
  auto id = ip.GetNextId();
  auto nested_task = tf.GetNextTask();
  auto nested_id = ip.GetNextId();

  ts.Add(id, task);
  auto& added_task = ts.Find(id);
  added_task.Add(nested_id, nested_task);
  auto parent_task = *ts.Find(id);
  auto child_task = *ts.Find(id).Find(nested_id);

  EXPECT_EQ(task, parent_task);
  EXPECT_EQ(nested_task, child_task);
}

TEST_F(TaskStorageTest, DeepNestedAdd) {
  constexpr int kElems = 128;
  TaskStorage ts;
  TaskFactory tf;
  MockTaskIdProducer ip;

  std::vector<Task> tasks;
  std::vector<TaskId> ids;
  for (int i{0}; i != kElems; ++i) {
    tasks.push_back(tf.GetNextTask());
    ids.push_back(ip.GetNextId());
  }
  ts.Add(ids.front(), tasks.front());

  for (int i{0}, end{kElems - 1}; i != end; ++i) {
    auto& add_to = ts.Find(ids.at(i));
    add_to.Add(ids.at(i + 1), tasks.at(i + 1));
  }

  for (int i{0}, end{kElems - 1}; i != end; ++i) {
    EXPECT_EQ(*ts.Find(ids.at(i)).Find(ids.at(i + 1)), tasks.at(i + 1));
  }
}

TEST_F(TaskStorageTest, NestedDelete) {
  TaskStorage ts;
  TaskFactory tf;
  MockTaskIdProducer ip;

  auto task = tf.GetNextTask();
  auto id = ip.GetNextId();

  auto nested_task = tf.GetNextTask();
  auto nested_id = ip.GetNextId();

  ts.Add(id, task);
  ts.Find(id).Add(nested_id, nested_task);
  ts.Find(id).Delete(nested_id);

  auto parent_of_deletable = ts.Find(id);

  ASSERT_THROW(parent_of_deletable.Find(nested_id), std::runtime_error);
  ASSERT_THROW(ts.Find(nested_id), std::runtime_error);
}

TEST_F(TaskStorageTest, NestedIdShouldBeDeletedAlso) {
  TaskStorage ts;
  TaskFactory tf;
  MockTaskIdProducer ip;

  auto task = tf.GetNextTask();
  auto id = ip.GetNextId();

  auto nested_task = tf.GetNextTask();
  auto nested_id = ip.GetNextId();

  auto nested_nested_task = tf.GetNextTask();
  auto nested_nested_id = ip.GetNextId();

  ts.Add(id, task);
  ts.Find(id).Add(nested_id, nested_task);
  ts.Find(nested_id).Add(nested_nested_id, nested_nested_task);

  ts.Delete(id);
  ASSERT_THROW(ts.Find(nested_id), std::runtime_error);
  ASSERT_THROW(ts.Find(nested_nested_id), std::runtime_error);
}