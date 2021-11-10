#include <gtest/gtest.h>

#include <sstream>
#include <vector>

#include "Model/task_manager.h"

bool TaskEquality(const Task& lhs, const Task& rhs) {
  return lhs.GetTitle() == rhs.GetTitle() &&
         lhs.GetPriority() == rhs.GetPriority() &&
         lhs.GetDueDate() == rhs.GetDueDate();
}

Task::Date_t parse_date(std::string date) {
  tm timeinfo;
  std::string pattern{"%d/%m/%y"};
  strptime(date.c_str(), pattern.c_str(), &timeinfo);
  return std::chrono::system_clock::from_time_t(std::mktime(&timeinfo));
}

TEST(BasicCreationAndEdition, Tests) {
  TaskManager tm;
  auto task = Task::Create("Test task #1", Task::Priority::kHigh,
                           parse_date("08/11/2021"));

  tm.Add(task);

  for (const auto& i : tm.Show()) {
    ASSERT_TRUE(TaskEquality(task, i.second));
  }

  auto edited_task = Task::Create(
      "Edited test task #1", Task::Priority::kMedium, parse_date("09/12/2021"));
  tm.Edit(TaskId::Create(0), edited_task);

  for (const auto& i : tm.Show()) {
    ASSERT_TRUE(TaskEquality(edited_task, i.second));
  }
}

TEST(TestOrdering, Tests) {
  TaskManager tm;
  std::vector<Task> tv;
  TaskIdProducer id_prod;
  {
    std::string name;
    std::string date;

    std::stringstream ss;
    for (int i = 0; i != 20; ++i) {
      ss << "Test task #" << i;
      name = ss.str();
      if (i < 9) {
        ss << "0";
      }
      ss << i + 1 << "/11/2021";
      date = ss.str();
      Task task = Task::Create(name, Task::Priority::kHigh, parse_date(date));
      tm.Add(task);
      tv.push_back(task);
    }
  }

  {
    auto tm_map = tm.Show();
    auto mb = tm_map.cbegin(), me = tm_map.cend();
    auto vb = tv.cbegin(), ve = tv.cend();
    for (; mb != me && vb != ve; ++vb, ++mb) {
      ASSERT_TRUE(TaskEquality(mb->second, *vb));
      ASSERT_EQ(id_prod.GetNextId(), mb->first);
    }
  }
}

TEST(TestExceptions, Tests) {
  TaskManager tm;
  {
    std::string name;
    std::string date;

    std::stringstream ss;
    for (int i = 0; i != 20; ++i) {
      ss << "Test task #" << i;
      name = ss.str();
      if (i < 9) {
        ss << "0";
      }
      ss << i + 1 << "/11/2021";
      date = ss.str();
      Task task = Task::Create(name, Task::Priority::kHigh, parse_date(date));
      tm.Add(task);
    }
  }
  for (int i = 0; i != 20; ++i) {
    tm.Delete(TaskId::Create(i));
  }
  for (int i = 0; i != 20; ++i) {
    // NOLINTNEXTLINE
    ASSERT_THROW(tm.Delete(TaskId::Create(i)), std::runtime_error);
    // NOLINTNEXTLINE
    ASSERT_THROW(tm.Complete(TaskId::Create(i)), std::runtime_error);
    // NOLINTNEXTLINE
    ASSERT_THROW(
        tm.Edit(TaskId::Create(i), Task::Create("", Task::Priority::kHigh, {})),
        std::runtime_error);
  }
}
