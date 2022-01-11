#include <gtest/gtest.h>

#include "test_utils/ScenarioFramework.h"

class AddOutputTest : public ::testing::Test, protected ScenarioFramework {
 protected:
  void SetUp() override { return SetUpImpl(); }
};

TEST_F(AddOutputTest, AddOneTask) {
  std::string date = "11:38 17/09/2019";
  std::stringstream ss;
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&time), kDatePattern);
  std::string default_date = ss.str();
  auto [task_storage, output] =
      RunScenario({"a", "title", date, "", "", "y", "q"});

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
  };
  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}

TEST_F(AddOutputTest, AddTaskAndSubTask) {
  std::string date = "10:38 17/09/2019";
  std::string priority = Strings::to_string(Task::kMedium);
  std::string state = Strings::to_string(Task::kUncompleted);
  std::stringstream ss;
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&time), kDatePattern);
  std::string default_date = ss.str();
  auto [task_storage, output] =
      RunScenario({"a", "title", date, priority, state, "y", "a 0", "subtitle",
                   "", "", "", "y", "q"});

  task_manager::SolidTask t0 =
      *std::find_if(task_storage.cbegin(), task_storage.cend(),
                    [](const auto& i) { return i.task_id().id() == 0; });

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

      Strings::kAddSubtaskTo,
      Strings::ShowSolidTask(t0),

      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(date),
      Strings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(priority),
      Strings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      Strings::LeaveEmptyFor(state),
      Strings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      Strings::ShowId(std::to_string(1)),
      Strings::GetPrompt(""),
  };
  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}