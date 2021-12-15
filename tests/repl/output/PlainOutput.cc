#include "UsageFramework.h"

class PlainOutputTest : public ::testing::Test, protected UsageFramework {
 protected:
  void SetUp() override { return SetUpImpl(); }
};

TEST_F(PlainOutputTest, HelpMustAppear) {
  auto [task_storage, output] = RunScenario({"h", "q"});

  std::vector<std::string> desired_output{
      IostreamStrings::GetPrompt(""),
      IostreamStrings::kHelp,
      IostreamStrings::GetPrompt(""),
  };

  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}
TEST_F(PlainOutputTest, InvalidIdMustAppear) {
  auto [task_storage, output] = RunScenario({"a oqpw", "q"});

  std::vector<std::string> desired_output{
      IostreamStrings::GetPrompt(""),
      IostreamStrings::kInvalidId,
      IostreamStrings::GetPrompt(""),
  };

  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}

TEST_F(PlainOutputTest, MultipleIdMustAppear) {
  std::string date = "11:38 17/09/2019";
  std::stringstream ss;
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&time), kDatePattern);
  std::string default_date = ss.str();

  auto [task_storage, output] =
      RunScenario({"a", "t", "", "", "", "y", "a 0 1", "q"});

  std::vector<std::string> desired_output{
      IostreamStrings::GetPrompt(""),
      IostreamStrings::GetPrompt("title"),
      IostreamStrings::LeaveEmptyFor(default_date),
      IostreamStrings::GetPrompt("due date", kDatePattern),
      IostreamStrings::LeaveEmptyFor(IostreamStrings::to_string(Task::kLow)),
      IostreamStrings::GetPrompt("priority"),
      IostreamStrings::kInvalidState,
      IostreamStrings::LeaveEmptyFor(
          IostreamStrings::to_string(Task::kUncompleted)),
      IostreamStrings::GetPrompt("state"),
      IostreamStrings::ProceedTo("add"),
      IostreamStrings::ShowId(0),
      IostreamStrings::GetPrompt(""),

      IostreamStrings::kMultipleId,
      IostreamStrings::GetPrompt(""),
  };

  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}

TEST_F(PlainOutputTest, UnknownCommandMustAppear) {
  std::string date = "11:38 17/09/2019";
  std::stringstream ss;
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&time), kDatePattern);
  std::string default_date = ss.str();

  auto [task_storage, output] = RunScenario({"sheesh", "q"});

  std::vector<std::string> desired_output{
      IostreamStrings::GetPrompt(""),
      IostreamStrings::kUnknownCommand,
      IostreamStrings::GetPrompt(""),
  };

  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}

TEST_F(PlainOutputTest, NotPresentIdMustAppear) {
  std::string date = "11:38 17/09/2019";
  std::stringstream ss;
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&time), kDatePattern);
  std::string default_date = ss.str();

  auto [task_storage, output] = RunScenario({"e 0", "q"});

  std::vector<std::string> desired_output{
      IostreamStrings::GetPrompt(""),
      IostreamStrings::NotPresentId(0),
      IostreamStrings::GetPrompt(""),
  };

  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}

TEST_F(PlainOutputTest, RequiredIdMustAppear) {
  std::string date = "11:38 17/09/2019";
  std::stringstream ss;
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&time), kDatePattern);
  std::string default_date = ss.str();

  auto [task_storage, output] = RunScenario({"e", "q"});

  std::vector<std::string> desired_output{
      IostreamStrings::GetPrompt(""),
      IostreamStrings::kRequiredId,
      IostreamStrings::GetPrompt(""),
  };

  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}
