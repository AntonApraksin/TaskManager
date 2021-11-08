#include <gtest/gtest.h>

#include <sstream>
#include <vector>

#include "TaskManager.h"

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
  auto task = Task::Create("Test task #1", Task::Priority::HIGH,
                           parse_date("08/11/2021"));

  tm.Add(task);

  for (const auto& i : tm.Show()) {
    ASSERT_TRUE(TaskEquality(task, i.second));
  }

  auto edited_task = Task::Create("Edited test task #1", Task::Priority::MEDIUM,
                                  parse_date("09/12/2021"));
  tm.Edit(0, edited_task);

  for (const auto& i : tm.Show()) {
    ASSERT_TRUE(TaskEquality(edited_task, i.second));
  }
}

TEST(TestOrdering, Tests) {
  TaskManager tm;
  std::vector<Task> tv;
  {
    std::string name;
    std::string date;

    std::stringstream ss;
    for (int i = 0; i != 20; ++i) {
      ss << "Test task #" << i;
      name = std::move(std::move(ss).str());
      if (i < 9) {
        ss << "0";
      }
      ss << i + 1 << "/11/2021";
      date = std::move(std::move(ss).str());
      Task task = Task::Create(name, Task::Priority::HIGH, parse_date(date));
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
      name = std::move(std::move(ss).str());
      if (i < 9) {
        ss << "0";
      }
      ss << i + 1 << "/11/2021";
      date = std::move(std::move(ss).str());
      Task task = Task::Create(name, Task::Priority::HIGH, parse_date(date));
      tm.Add(task);
    }
  }
  for (int i = 0; i != 20; ++i) {
    tm.Delete(i);
  }
  for (int i = 0; i != 20; ++i) {
    ASSERT_THROW(tm.Delete(i), std::runtime_error);
    ASSERT_THROW(tm.Complete(i), std::runtime_error);
    ASSERT_THROW(tm.Edit(i, Task::Create("", Task::Priority::HIGH, {})),
                 std::runtime_error);
  }
}
