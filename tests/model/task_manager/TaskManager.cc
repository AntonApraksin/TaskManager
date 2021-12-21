#include "model/task_manager/TaskManager.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../ModelUtils.h"

using ::testing::AtLeast;
using ::testing::Return;

std::unique_ptr<ITaskIdProducer> get_default_task_id_producer() {
  return std::make_unique<MockTaskIdProducer>();
}

class PlainTaskManagerTest : public ::testing::Test {};

TEST_F(PlainTaskManagerTest, TaskAddedProperly) {
  auto id_producer = get_default_task_id_producer();
  TaskFactory tf;
  TaskManager tm{std::move(id_producer)};
  auto task = tf.GetNextTask();
  auto result = tm.Add(task);
  ASSERT_EQ(result.GetStatus(), TaskManager::Status::kOk);
  ASSERT_EQ(tm.Show().tasks.size(), 1);
  ASSERT_EQ(tm.Show().parents.size(), 1);
  ASSERT_EQ(tm.Show().roots.size(), 1);
  EXPECT_EQ(task, tm.Show().tasks[tm.Show().roots[0]]);
}

TEST_F(PlainTaskManagerTest, InvalidIdResultOnDeleteWithUnexistingId) {
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  auto task = tf.GetNextTask();
  auto _ = tm.Add(task);
  auto tmp_id = tm.Show().roots[0];
  tm.Delete(tmp_id);
  EXPECT_EQ(tm.Delete(tmp_id).GetStatus(), TaskManager::Status::kNotPresentId);
}

TEST_F(PlainTaskManagerTest, InvalidIdResultOnCompleteWithUnexistingId) {
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  auto task = tf.GetNextTask();
  tm.Add(task);
  auto tmp_id = tm.Show().roots[0];
  tm.Delete(tmp_id);
  EXPECT_EQ(tm.Complete(tmp_id).GetStatus(),
            TaskManager::Status::kNotPresentId);
}

TEST_F(PlainTaskManagerTest, InvalidIdResultOnEditWithUnexistingId) {
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  auto task = tf.GetNextTask();
  tm.Add(task);
  auto tmp_id = tm.Show().roots[0];
  tm.Delete(tmp_id);
  EXPECT_EQ(tm.Edit(tmp_id, task).GetStatus(),
            TaskManager::Status::kNotPresentId);
}

TEST_F(PlainTaskManagerTest, ProperDeletion) {
  constexpr int kElems = 512;
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;

  for (int i = 0; i != kElems; ++i) {
    auto task = tf.GetNextTask();
    tm.Add(task);
  }
  auto storage = tm.Show();
  ASSERT_EQ(storage.parents.size(), kElems);
  ASSERT_EQ(storage.tasks.size(), kElems);
  ASSERT_EQ(storage.roots.size(), kElems);

  for (const auto& i : storage.roots) {
    tm.Delete(i);
  }
  storage = tm.Show();

  ASSERT_EQ(storage.parents.size(), 0);
  ASSERT_EQ(storage.tasks.size(), 0);
  ASSERT_EQ(storage.roots.size(), 0);
}

TEST_F(PlainTaskManagerTest, ProperCompletion) {
  constexpr int kElems = 512;
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;

  for (int i = 0; i != kElems; ++i) {
    auto task = tf.GetNextTask();
    tm.Add(task);
  }
  auto storage = tm.Show();

  for (const auto& i : storage.roots) {
    tm.Complete(i);
  }

  storage = tm.Show();
  for (const auto& i : storage.roots) {
    ASSERT_EQ(storage.tasks[i].progress(), Task::kCompleted);
  }
}

TEST_F(PlainTaskManagerTest, ProperEdition) {
  constexpr int kElems = 256;
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  std::vector<Task> vec_tasks;

  for (int i = 0; i != kElems; ++i) {
    auto task = tf.GetNextTask();
    auto new_task = tf.GetNextTask();
    tm.Add(task);
    auto storage = tm.Show();
    auto res = std::find_if(storage.tasks.cbegin(), storage.tasks.cend(),
                            [task](const auto& t) { return t.second == task; });
    tm.Edit(res->first, new_task);
    vec_tasks.push_back(new_task);
  }

  for (const auto& i : vec_tasks) {
    auto storage = tm.Show();
    auto iter = std::find_if(storage.tasks.cbegin(), storage.tasks.cend(),
                             [i](const auto& t) { return t.second == i; });
    tm.Delete(iter->first);
  }

  auto storage = tm.Show();

  ASSERT_EQ(storage.parents.size(), 0);
  ASSERT_EQ(storage.tasks.size(), 0);
  ASSERT_EQ(storage.roots.size(), 0);
}

// TODO: Add tests for nested things