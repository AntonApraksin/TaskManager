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
  auto got = *ts.Find(id)->second;

  EXPECT_EQ(task, got);
}

TEST_F(TaskStorageTest, SimpleDelete) {
  TaskStorage ts;
  TaskFactory tf;
  MockTaskIdProducer ip;

  auto task = tf.GetNextTask();
  auto id = ip.GetNextId();

  ts.Add(id, task);

  auto parent_of_deletable = ts.FindStorageContaining(id);
  parent_of_deletable->get().Delete(parent_of_deletable->get().Find(id));
  EXPECT_EQ(ts.Find(id), ts.end());
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
  ts.Find(id)->second.Add(nested_id, nested_task);

  ASSERT_EQ(*ts.FindStorageContaining(nested_id)->get().Find(nested_id)->second,
            nested_task);
}

TEST_F(TaskStorageTest, FindOnUnexistingIdShouldResultInIterEnd) {
  TaskStorage ts;
  MockTaskIdProducer ip;

  auto id = ip.GetNextId();

  EXPECT_EQ(ts.Find(id), ts.end());
}

TEST_F(TaskStorageTest, FindParentOfUnexistingIdShouldResultInNullopt) {
  TaskStorage ts;
  MockTaskIdProducer ip;

  auto id = ip.GetNextId();

  EXPECT_EQ(ts.FindStorageContaining(id), std::nullopt);
}

TEST_F(TaskStorageTest, FindShouldWorkOnConstantObjects) {
  TaskStorage ts;
  TaskFactory tf;
  MockTaskIdProducer ip;

  auto id = ip.GetNextId();
  auto task = tf.GetNextTask();

  ts.Add(id, task);
  auto const constant_ts = ts;

  ASSERT_EQ(*constant_ts.Find(id)->second, task);
}

TEST_F(TaskStorageTest, FindParentOfShouldWorkOnConstantObjects) {
  TaskStorage ts;
  TaskFactory tf;
  MockTaskIdProducer ip;

  auto id = ip.GetNextId();
  auto task = tf.GetNextTask();

  ts.Add(id, task);
  auto const constant_ts = ts;

  ASSERT_EQ(*constant_ts.FindStorageContaining(id)->get().Find(id)->second,
            task);
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
  ts.Find(id)->second.Add(nested_id, nested_task);

  auto const constant_ts = ts;
  ASSERT_EQ(*constant_ts.FindStorageContaining(nested_id)
                 ->get()
                 .Find(nested_id)
                 ->second,
            nested_task);
  ASSERT_EQ(*constant_ts.Find(nested_id)->second, nested_task);
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
  auto added_task = ts.Find(id);
  added_task->second.Add(nested_id, nested_task);
  auto parent_task = *ts.Find(id)->second;
  auto child_task = *ts.Find(id)->second.Find(nested_id)->second;

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
    auto add_to = ts.Find(ids.at(i));
    add_to->second.Add(ids.at(i + 1), tasks.at(i + 1));
  }

  for (int i{0}, end{kElems - 1}; i != end; ++i) {
    EXPECT_EQ(*ts.Find(ids.at(i))->second.Find(ids.at(i + 1))->second,
              tasks.at(i + 1));
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
  ts.Find(id)->second.Add(nested_id, nested_task);
  ts.Find(id)->second.Delete(ts.Find(id)->second.Find(nested_id));

  auto parent_of_deletable = ts.Find(id);

  EXPECT_EQ(parent_of_deletable->second.Find(nested_id),
            parent_of_deletable->second.end());
  EXPECT_EQ(ts.Find(nested_id), ts.end());
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
  ts.Find(id)->second.Add(nested_id, nested_task);
  ts.Find(nested_id)->second.Add(nested_nested_id, nested_nested_task);

  ts.Delete(ts.Find(id));
  EXPECT_EQ(ts.Find(nested_id), ts.end());
  EXPECT_EQ(ts.Find(nested_nested_id), ts.end());
}