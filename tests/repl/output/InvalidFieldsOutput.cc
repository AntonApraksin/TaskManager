#include "UsageFramework.h"

#if 0
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
      Strings::GetPrompt(""),
      Strings::GetPrompt("title"),
      Strings::kTitleMustNotBeEmpty,
      Strings::GetPrompt("title"),
      IostreamStrings::kTitleMustNotBeEmpty,
      Strings::GetPrompt("title"),

      Strings::LeaveEmptyFor(default_date),
      Strings::GetPrompt("due date", kDatePattern),
      Strings::kWrongDateFormat,
      IostreamStrings::GetPrompt("due date", kDatePattern),
      IostreamStrings::kWrongDateFormat,
      IostreamStrings::GetPrompt("due date", kDatePattern),

      IostreamStrings::LeaveEmptyFor(Strings::to_string(Task::kLow)),
      IostreamStrings::GetPrompt("priority"),
      Strings::kPriorityShouldBe,
      IostreamStrings::GetPrompt("priority"),
      Strings::kPriorityShouldBe,
      Strings::GetPrompt("priority"),

      Strings::kStateShouldBe,
      IostreamStrings::LeaveEmptyFor(
          Strings::to_string(Task::kUncompleted)),
      Strings::GetPrompt("state"),
      IostreamStrings::kStateShouldBe,
      IostreamStrings::GetPrompt("state"),
      Strings::kStateShouldBe,
      Strings::GetPrompt("state"),

      Strings::ProceedTo("add"),
      Strings::ShowId(std::to_string(0)),
      IostreamStrings::GetPrompt(""),
  };
  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}
#endif