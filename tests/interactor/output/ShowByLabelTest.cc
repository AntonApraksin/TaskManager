#include <gtest/gtest.h>

#include "test_utils/ScenarioFramework.h"

class ShowByLabelTest : public ::testing::Test, protected ScenarioFramework {
 protected:
  void SetUp() override { return SetUpImpl(); }
};

TEST_F(ShowByLabelTest, MustShowTasksWithLabel) {
  std::stringstream ss;
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&time), kDatePattern);

  std::string default_date = ss.str();
  auto t1 = task_stringed_data_producer_.GetData();
  auto t2 = task_stringed_data_producer_.GetData();
  auto t3 = task_stringed_data_producer_.GetData();
  auto to_show1 = TaskDataToSolidTask(t1, 0);
  to_show1.mutable_task()->add_labels()->set_name("foo");
  auto to_show2 = TaskDataToSolidTask(t2, 1);
  to_show2.mutable_task()->add_labels()->set_name("foo");
  SolidTasks to_show{to_show2, to_show1};
  auto [task_storage, output] = RunScenario(
      {"a",        t1.title, t1.date,    t1.priority, t1.state, "y",
       "a",        t2.title, t2.date,    t2.priority, t2.state, "y",
       "a",        t3.title, t3.date,    t3.priority, t3.state, "y",
       "al 0 foo", "y",      "al 1 foo", "y",         "s foo",  "q"});

  std::vector<std::string> desired_output{
      Strings::GetPrompt(""),
      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(default_date),
      Strings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(Strings::to_string(Task::kLow)),
      Strings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      Strings::LeaveEmptyFor(Strings::to_string(Task::kUncompleted)),
      Strings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      Strings::ShowId(std::to_string(0)),
      Strings::GetPrompt(""),
      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(default_date),
      Strings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(Strings::to_string(Task::kLow)),
      Strings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      Strings::LeaveEmptyFor(Strings::to_string(Task::kUncompleted)),
      Strings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      Strings::ShowId(std::to_string(1)),
      Strings::GetPrompt(""),
      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(default_date),
      Strings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(Strings::to_string(Task::kLow)),
      Strings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      Strings::LeaveEmptyFor(Strings::to_string(Task::kUncompleted)),
      Strings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      Strings::ShowId(std::to_string(2)),
      Strings::GetPrompt(""),
      Strings::ProceedTo("add label"),
      Strings::GetPrompt(""),
      Strings::ProceedTo("add label"),
      Strings::GetPrompt(""),
      Strings::ShowSolidTasksWithoutNest(to_show),
      Strings::GetPrompt("")};

  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}