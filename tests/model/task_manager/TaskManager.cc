#include "model/task_manager/TaskManager.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "test_utils/MockTaskIdProducer.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/operators.h"

using namespace task_manager;  // NOLINT: Testing purpose

using ::testing::AtLeast;
using ::testing::Return;

std::unique_ptr<ITaskIdProducer> get_default_task_id_producer() {
  return std::make_unique<MockTaskIdProducer>();
}

class TaskManagerTest : public ::testing::Test {};

TEST_F(TaskManagerTest, TaskAddedProperly) {
  auto id_producer = get_default_task_id_producer();
  TaskFactory tf;
  TaskManager tm{std::move(id_producer)};
  auto task = tf.GetNextTask();
  auto result = tm.Add(task);
  ASSERT_EQ(result.GetStatus(), TaskManager::Status::kOk);
  auto storage = tm.Show().AccessResult();
  ASSERT_EQ(storage.tasks.size(), 1);
  ASSERT_EQ(storage.parents.size(), 1);
  ASSERT_EQ(storage.roots.size(), 1);
  EXPECT_EQ(task, storage.tasks[storage.roots[0]]);
}

TEST_F(TaskManagerTest, InvalidIdResultOnDeleteWithUnexistingId) {
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  auto task = tf.GetNextTask();
  auto _ = tm.Add(task);
  auto storage = tm.Show().AccessResult();
  auto tmp_id = storage.roots[0];
  tm.Delete(tmp_id);
  EXPECT_EQ(tm.Delete(tmp_id).GetStatus(), TaskManager::Status::kNotPresentId);
}

TEST_F(TaskManagerTest, InvalidIdResultOnCompleteWithUnexistingId) {
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  auto task = tf.GetNextTask();
  tm.Add(task);
  auto storage = tm.Show().AccessResult();
  auto tmp_id = storage.roots[0];
  tm.Delete(tmp_id);
  EXPECT_EQ(tm.Complete(tmp_id).GetStatus(),
            TaskManager::Status::kNotPresentId);
}

TEST_F(TaskManagerTest, InvalidIdResultOnEditWithUnexistingId) {
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  auto task = tf.GetNextTask();
  tm.Add(task);
  auto storage = tm.Show().AccessResult();
  auto tmp_id = storage.roots[0];
  tm.Delete(tmp_id);
  EXPECT_EQ(tm.Edit(tmp_id, task).GetStatus(),
            TaskManager::Status::kNotPresentId);
}

TEST_F(TaskManagerTest, ProperDeletion) {
  constexpr int kElems = 512;
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;

  for (int i = 0; i != kElems; ++i) {
    auto task = tf.GetNextTask();
    tm.Add(task);
  }
  auto storage = tm.Show();
  ASSERT_EQ(storage.AccessResult().parents.size(), kElems);
  ASSERT_EQ(storage.AccessResult().tasks.size(), kElems);
  ASSERT_EQ(storage.AccessResult().roots.size(), kElems);

  for (const auto& i : storage.AccessResult().roots) {
    tm.Delete(i);
  }
  storage = tm.Show();

  ASSERT_EQ(storage.AccessResult().parents.size(), 0);
  ASSERT_EQ(storage.AccessResult().tasks.size(), 0);
  ASSERT_EQ(storage.AccessResult().roots.size(), 0);
}

TEST_F(TaskManagerTest, ProperCompletion) {
  constexpr int kElems = 512;
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;

  for (int i = 0; i != kElems; ++i) {
    auto task = tf.GetNextTask();
    tm.Add(task);
  }
  auto storage = tm.Show().AccessResult();

  for (const auto& i : storage.roots) {
    tm.Complete(i);
  }

  storage = tm.Show().AccessResult();
  for (const auto& i : storage.roots) {
    ASSERT_EQ(storage.tasks[i].progress(), Task::kCompleted);
  }
}

TEST_F(TaskManagerTest, ProperEdition) {
  constexpr int kElems = 256;
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  std::vector<Task> vec_tasks;

  for (int i = 0; i != kElems; ++i) {
    auto task = tf.GetNextTask();
    auto new_task = tf.GetNextTask();
    tm.Add(task);
    auto storage = tm.Show().AccessResult();
    auto res = std::find_if(storage.tasks.cbegin(), storage.tasks.cend(),
                            [task](const auto& t) { return t.second == task; });
    tm.Edit(res->first, new_task);
    vec_tasks.push_back(new_task);
  }

  for (const auto& i : vec_tasks) {
    auto storage = tm.Show().AccessResult();
    auto iter = std::find_if(storage.tasks.cbegin(), storage.tasks.cend(),
                             [i](const auto& t) { return t.second == i; });
    tm.Delete(iter->first);
  }

  auto storage = tm.Show().AccessResult();

  ASSERT_EQ(storage.parents.size(), 0);
  ASSERT_EQ(storage.tasks.size(), 0);
  ASSERT_EQ(storage.roots.size(), 0);
}

TEST_F(TaskManagerTest, MustAddLabel) {
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  auto task = tf.GetNextTask();
  auto id = tm.Add(task).AccessResult();
  Label label;
  label.set_name("label1");
  auto result = tm.AddLabel(id, label);
  ASSERT_EQ(result.GetStatus(), TaskManager::Status::kOk);
  auto storage = tm.Show().AccessResult();
  auto got_task = storage.tasks.at(id);
  ASSERT_EQ(got_task.labels_size(), 1);
  EXPECT_EQ(got_task.labels()[0].name(), label.name());
}

TEST_F(TaskManagerTest, MustDeleteLabel) {
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  auto task = tf.GetNextTask();
  auto id = tm.Add(task).AccessResult();
  Label label;
  label.set_name("label1");
  auto result = tm.AddLabel(id, label);
  ASSERT_EQ(result.GetStatus(), TaskManager::Status::kOk);
  result = tm.DeleteLabel(id, label);
  ASSERT_EQ(result.GetStatus(), TaskManager::Status::kOk);
  auto storage = tm.Show().AccessResult();
  auto got_task = storage.tasks.at(id);
  ASSERT_EQ(got_task.labels_size(), 0);
}

TEST_F(TaskManagerTest, AddLabelWithNotPresentIdMustResultInkNotPresentId) {
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  auto task = tf.GetNextTask();
  auto id = tm.Add(task).AccessResult();
  tm.Delete(id);
  Label label;
  label.set_name("label1");
  auto result = tm.AddLabel(id, label);
  ASSERT_EQ(result.GetStatus(), TaskManager::Status::kNotPresentId);
}

TEST_F(TaskManagerTest, AddLabelWithRepeatedLabelMustNotAddItAndReturnkOk) {
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  auto task = tf.GetNextTask();
  auto id = tm.Add(task).AccessResult();
  Label label;
  label.set_name("label1");
  auto result = tm.AddLabel(id, label);
  ASSERT_EQ(result.GetStatus(), TaskManager::Status::kOk);
  result = tm.AddLabel(id, label);
  ASSERT_EQ(result.GetStatus(), TaskManager::Status::kOk);
  auto storage = tm.Show().AccessResult();
  auto got_task = storage.tasks.at(id);
  ASSERT_EQ(got_task.labels_size(), 1);
  EXPECT_EQ(got_task.labels()[0].name(), label.name());
}

TEST_F(TaskManagerTest, DeleteLabelWithNotPresentIdMustResultInkNotPresentId) {
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  auto task = tf.GetNextTask();
  auto id = tm.Add(task).AccessResult();
  tm.Delete(id);
  Label label;
  label.set_name("label1");
  auto result = tm.DeleteLabel(id, label);
  ASSERT_EQ(result.GetStatus(), TaskManager::Status::kNotPresentId);
}

TEST_F(TaskManagerTest,
       DeleteLabelWithNotPresentLabelMustResultInkNotPresentLabel) {
  TaskManager tm{get_default_task_id_producer()};
  TaskFactory tf;
  auto task = tf.GetNextTask();
  auto id = tm.Add(task).AccessResult();
  Label label;
  label.set_name("label1");
  auto result = tm.DeleteLabel(id, label);
  ASSERT_EQ(result.GetStatus(), TaskManager::Status::kNotPresentLabel);
}
// TODO: Add tests for nested things