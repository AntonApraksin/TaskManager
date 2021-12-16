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

TEST_F(PlainTaskManagerTest, InvalidIdResultOnDeleteWithUnexistingId) {
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  auto task = tf.GetNextTask();
  tm.Add(task);
  auto tmp_id = tm.Show().cbegin()->first;
  tm.Delete(tmp_id);
  EXPECT_EQ(tm.Delete(tmp_id).status,
            TaskManager::ActionResult::Status::kNotPresentId);
}

TEST_F(PlainTaskManagerTest, InvalidIdResultOnCompleteWithUnexistingId) {
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  auto task = tf.GetNextTask();
  tm.Add(task);
  auto tmp_id = tm.Show().ShowStorage().cbegin()->first;
  tm.Delete(tmp_id);
  EXPECT_EQ(tm.Complete(tmp_id).status,
            TaskManager::ActionResult::Status::kNotPresentId);
}

TEST_F(PlainTaskManagerTest, InvalidIdResultOnEditWithUnexistingId) {
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  auto task = tf.GetNextTask();
  tm.Add(task);
  auto tmp_id = tm.Show().ShowStorage().cbegin()->first;
  tm.Delete(tmp_id);
  EXPECT_EQ(tm.Edit(tmp_id, task).status,
            TaskManager::ActionResult::Status::kNotPresentId);
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

  auto storage = tm.Show().ShowStorage();
  for (const auto& i : storage) {
    ASSERT_EQ(i.second->progress(), Task::kCompleted);
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
