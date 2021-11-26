#include "model/task_wrapper/TaskWrapper.h"

#include <gtest/gtest.h>

#include "model/id/ITaskIdProducer.h"

bool operator==(const Task& lhs, const Task& rhs);

class MockTaskIdProducer : public ITaskIdProducer {
 public:
  TaskId GetNextId() override { return TaskId::Create(current_++); }

 private:
  int current_ = 0;
};

class TaskFactory final {
 public:
  Task GetNextTask() {
    std::stringstream ss;
    ss << "Sample task #" << state_;
    Date_t time =
        std::chrono::system_clock::now() + std::chrono::seconds(state_);
    Task::Priority priority = static_cast<Task::Priority>(state_ % 4);
    Task::State state = static_cast<Task::State>(state_ % 3);
    ++state_;
    return *Task::Create(ss.str(), priority, time, state);
  }

 private:
  int state_ = 0;
};

class TaskStorageTest : public ::testing::Test {};

TEST_F(TaskStorageTest, PlainAdd) {
  TaskStorage ts;
  TaskFactory tf;
  MockTaskIdProducer ip;

  auto task = tf.GetNextTask();
  auto id = ip.GetNextId();
  ts.Add(id, task);
  auto got = *ts.Find(id);

  EXPECT_EQ(task, got);
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