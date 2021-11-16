#include "Model/task_manager.h"

#include <gtest/gtest.h>

Task::Date_t parse_date(std::string date) {
  tm timeinfo;
  std::string pattern{"%d/%m/%y"};
  strptime(date.c_str(), pattern.c_str(), &timeinfo);
  return std::chrono::system_clock::from_time_t(std::mktime(&timeinfo));
}

Task CreateSampleTask()
{
  std::string task_title = "Test task title";
  Task::Date_t task_due_date = parse_date("03/11/2020");
  Task::Priority task_priority = Task::Priority::kMedium;
  return Task::Create(task_title, task_priority, task_due_date);
}

bool operator==(const Task& lhs, const Task& rhs) {
  return lhs.GetTitle() == rhs.GetTitle() &&
         lhs.GetPriority() == rhs.GetPriority() &&
         lhs.GetDueDate() == rhs.GetDueDate();
}

class TaskManagerTest : public ::testing::Test {};

TEST_F(TaskManagerTest, TaskAddedProperly) {
  TaskManager tm{};
  auto task = CreateSampleTask();
  tm.Add(task);
  ASSERT_EQ(tm.Show().size(), 1);
  EXPECT_EQ(task, tm.Show().cbegin()->second);
}

TEST_F(TaskManagerTest, RuntimeErrorOnDeleteWithUnexistingId) {
  TaskManager tm{};
  auto task = CreateSampleTask();
  tm.Add(task);
  auto tmp_id = tm.Show().cbegin()->first;
  tm.Delete(tmp_id);
  // NOLINTNEXTLINE
  EXPECT_THROW(tm.Delete(tmp_id), std::runtime_error);
}

TEST_F(TaskManagerTest, RuntimeErrorOnCompleteWithUnexistingId) {
  TaskManager tm{};
  auto task = CreateSampleTask();
  tm.Add(task);
  auto tmp_id = tm.Show().cbegin()->first;
  tm.Delete(tmp_id);
  // NOLINTNEXTLINE
  EXPECT_THROW(tm.Delete(tmp_id), std::runtime_error);
}

TEST_F(TaskManagerTest, RuntimeErrorOnEditWithUnexistingId) {
  TaskManager tm{};
  std::string task_title = "Test task title";
  Task::Date_t task_due_date = parse_date("03/11/2020");
  Task::Priority task_priority = Task::Priority::kMedium;
  auto task = Task::Create(task_title, task_priority, task_due_date);
  tm.Add(task);
  auto tmp_id = tm.Show().cbegin()->first;
  tm.Delete(tmp_id);
  // NOLINTNEXTLINE
  EXPECT_THROW(tm.Edit(tmp_id, task), std::runtime_error);
}

TEST_F(TaskManagerTest, ProperDeletion) {
  constexpr int kElems = 512;
  TaskManager tm;

  {
    std::stringstream ss;
    for (int i = 0; i != kElems; ++i) {
      ss << "Test task #" << i;
      Task task = Task::Create(ss.str(), Task::Priority::kHigh, {});
      tm.Add(task);
    }
  }
  auto tasks_map = tm.Show();
  EXPECT_EQ(tasks_map.size(), 512);

  for (const auto& i : tasks_map) {
    tm.Delete(i.first);
  }

  EXPECT_TRUE(tm.Show().empty());
}

TEST_F(TaskManagerTest, ProperEdition) {
  constexpr int kElems = 512;
  TaskManager tm;
  std::vector<Task> vec_tasks;

  {
    std::stringstream ss;
    for (int i = 0; i != kElems;) {
      ss << "Test task #" << i++;
      Task task = Task::Create(ss.str(), Task::Priority::kHigh, {});
      ss << "Test task #" << i++;
      Task new_task = Task::Create(ss.str(), Task::Priority::kHigh, {});
      tm.Add(task);
      auto res =
          std::find_if(tm.Show().cbegin(), tm.Show().cend(),
                       [task](const auto& t) { return t.second == task; });
      tm.Edit(res->first, new_task);
      vec_tasks.push_back(new_task);
    }
  }

  for (const auto& i : vec_tasks) {
    auto iter = std::find_if(tm.Show().cbegin(), tm.Show().cend(),
                             [i](const auto& t) { return t.second == i; });
    tm.Delete(iter->first);
  }

  EXPECT_TRUE(tm.Show().empty());
}
