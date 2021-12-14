#include "UsageFramework.h"

class InvalidFieldsOutputTest : public ::testing::Test,
                                protected UsageFramework {
 protected:
  void SetUp() override { return SetUpImpl(); }
};

TEST_F(InvalidFieldsOutputTest, AddAction) {
  std::stringstream ss;
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&time), kDatePattern);
  std::string default_date = ss.str();
  auto [task_storage, output] =
      RunScenario({"a", /*title:*/ "", "", "title", /*date:*/ "12", "fsdf", "",
                   /*priority:*/ "inv", "sheesh", "",
                   /*state:*/ "rep", "1", "", "y", "q"});

  std::vector<std::string> desired_output{
      IostreamStrings::GetPrompt(""),
      IostreamStrings::GetPrompt("title"),
      IostreamStrings::kTitleMustNotBeEmpty,
      IostreamStrings::GetPrompt("title"),
      IostreamStrings::kTitleMustNotBeEmpty,
      IostreamStrings::GetPrompt("title"),

      IostreamStrings::LeaveEmptyFor(default_date),
      IostreamStrings::GetPrompt("due date", kDatePattern),
      IostreamStrings::kInvalidDate,
      IostreamStrings::GetPrompt("due date", kDatePattern),
      IostreamStrings::kInvalidDate,
      IostreamStrings::GetPrompt("due date", kDatePattern),

      IostreamStrings::LeaveEmptyFor(
          IostreamStrings::to_string(Task::Priority::kLow)),
      IostreamStrings::GetPrompt("priority"),
      IostreamStrings::kInvalidPriority,
      IostreamStrings::GetPrompt("priority"),
      IostreamStrings::kInvalidPriority,
      IostreamStrings::GetPrompt("priority"),

      IostreamStrings::kInvalidState,
      IostreamStrings::LeaveEmptyFor(
          IostreamStrings::to_string(Task::State::kUncompleted)),
      IostreamStrings::GetPrompt("state"),
      IostreamStrings::kInvalidState,
      IostreamStrings::GetPrompt("state"),
      IostreamStrings::kInvalidState,
      IostreamStrings::GetPrompt("state"),

      IostreamStrings::ProceedTo("add"),
      IostreamStrings::ShowId(0),
      IostreamStrings::GetPrompt(""),
  };
  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}
