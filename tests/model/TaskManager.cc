#include "model/task_manager/TaskManager.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ModelUtils.h"

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
  tm.Add(task);
  ASSERT_EQ(tm.Show().ShowStorage().size(), 1);
  EXPECT_EQ(task, *tm.Show().ShowStorage().cbegin()->second);
}

TEST_F(PlainTaskManagerTest, RuntimeErrorOnDeleteWithUnexistingId) {
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  auto task = tf.GetNextTask();
  tm.Add(task);
  auto tmp_id = tm.Show().ShowStorage().cbegin()->first;
  tm.Delete(tmp_id);
  // NOLINTNEXTLINE
  EXPECT_THROW(tm.Delete(tmp_id), std::runtime_error);
}

TEST_F(PlainTaskManagerTest, RuntimeErrorOnCompleteWithUnexistingId) {
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  auto task = tf.GetNextTask();
  tm.Add(task);
  auto tmp_id = tm.Show().ShowStorage().cbegin()->first;
  tm.Delete(tmp_id);
  // NOLINTNEXTLINE
  EXPECT_THROW(tm.Delete(tmp_id), std::runtime_error);
}

TEST_F(PlainTaskManagerTest, RuntimeErrorOnEditWithUnexistingId) {
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  auto task = tf.GetNextTask();
  tm.Add(task);
  auto tmp_id = tm.Show().ShowStorage().cbegin()->first;
  tm.Delete(tmp_id);
  // NOLINTNEXTLINE
  EXPECT_THROW(tm.Edit(tmp_id, task), std::runtime_error);
}

TEST_F(PlainTaskManagerTest, ProperDeletion) {
  constexpr int kElems = 512;
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;

  for (int i = 0; i != kElems; ++i) {
    auto task = tf.GetNextTask();
    tm.Add(task);
  }
  auto tasks_map = tm.Show().ShowStorage();
  EXPECT_EQ(tasks_map.size(), kElems);

  for (const auto& i : tasks_map) {
    tm.Delete(i.first);
  }

  EXPECT_EQ(tm.Show().ShowStorage().size(), 0);
}

TEST_F(PlainTaskManagerTest, ProperCompletion) {
  constexpr int kElems = 512;
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;

  for (int i = 0; i != kElems; ++i) {
    auto task = tf.GetNextTask();
    tm.Add(task);
  }
  auto tasks_map = tm.Show().ShowStorage();

  for (const auto& i : tasks_map) {
    tm.Complete(i.first);
  }

  for (const auto& i : tm.Show().ShowStorage()) {
    ASSERT_EQ(i.second->GetState(), Task::State::kCompleted);
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
    auto res = std::find_if(
        tm.Show().ShowStorage().cbegin(), tm.Show().ShowStorage().cend(),
        [task](const auto& t) { return *t.second == task; });
    tm.Edit(res->first, new_task);
    vec_tasks.push_back(new_task);
  }

  for (const auto& i : vec_tasks) {
    auto iter = std::find_if(tm.Show().ShowStorage().cbegin(),
                             tm.Show().ShowStorage().cend(),
                             [i](const auto& t) { return *t.second == i; });
    tm.Delete(iter->first);
  }

  EXPECT_TRUE(tm.Show().ShowStorage().empty());
}
