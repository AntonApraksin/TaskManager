#include "UsageFramework.h"

#if 0

class AddOutputTest : public ::testing::Test, protected UsageFramework {
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
      IostreamStrings::LeaveEmptyFor(Strings::to_string(Task::kLow)),
      Strings::GetPrompt("priority"),
      IostreamStrings::kStateShouldBe,
      IostreamStrings::LeaveEmptyFor(
          IostreamStrings::to_string(Task::kUncompleted)),
      IostreamStrings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      IostreamStrings::ShowId(std::to_string(0)),
      Strings::GetPrompt(""),
  };
  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}

TEST_F(AddOutputTest, AddTaskAndSubTask) {
  std::string date = "10:38 17/09/2019";
  std::string priority = IostreamStrings::to_string(Task::kMedium);
  std::string state = Strings::to_string(Task::kUncompleted);
  std::stringstream ss;
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&time), kDatePattern);
  std::string default_date = ss.str();
  auto [task_storage, output] =
      RunScenario({"a", "title", date, priority, state, "y", "a 0", "subtitle",
                   "", "", "", "y", "q"});

  std::vector<std::string> desired_output{
      IostreamStrings::GetPrompt(""),
      IostreamStrings::GetPrompt("title"),
      IostreamStrings::LeaveEmptyFor(default_date),
      Strings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(IostreamStrings::to_string(Task::kLow)),
      IostreamStrings::GetPrompt("priority"),
      IostreamStrings::kStateShouldBe,
      IostreamStrings::LeaveEmptyFor(
          IostreamStrings::to_string(Task::kUncompleted)),
      Strings::GetPrompt("state"),
      IostreamStrings::ProceedTo("add"),
      IostreamStrings::ShowId(std::to_string(0)),
      Strings::GetPrompt(""),

      IostreamStrings::kAddSubtaskTo,
      Strings::ShowTask(*task_storage.Find(CreateTaskId(0))->second),

      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(date),
      IostreamStrings::GetPrompt("due date", kDatePattern),
      IostreamStrings::LeaveEmptyFor(priority),
      IostreamStrings::GetPrompt("priority"),
      Strings::kInvalidState,
      IostreamStrings::LeaveEmptyFor(state),
      IostreamStrings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      Strings::ShowId(std::to_string(1)),
      Strings::GetPrompt(""),
  };
  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}
#endif