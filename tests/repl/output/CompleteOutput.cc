#include "UsageFramework.h"

#if 0

class CompleteOutputTest : public ::testing::Test, protected UsageFramework {
 protected:
  void SetUp() override { return SetUpImpl(); }
};

TEST_F(CompleteOutputTest, RewriteItEveryTimeYouChangeTheSourceCode) {
  std::stringstream ss;
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&time), kDatePattern);

  auto t1 = task_stringed_data_producer_.GetData();
  auto sub_t1 = task_stringed_data_producer_.GetData();
  auto sub_sub_t1 = task_stringed_data_producer_.GetData();
  auto [task_storage, output] = RunScenario({"a",
                                             t1.title,
                                             t1.date,
                                             t1.priority,
                                             t1.state,
                                             "y",
                                             "a 0",
                                             sub_t1.title,
                                             sub_t1.date,
                                             sub_t1.priority,
                                             sub_t1.state,
                                             "y",
                                             "a 1",
                                             sub_sub_t1.title,
                                             sub_sub_t1.date,
                                             sub_sub_t1.priority,
                                             sub_sub_t1.state,
                                             "y",
                                             "c 0",
                                             "y",
                                             "q"});
  std::string default_date = ss.str();

  std::vector<std::string> desired_output{
      Strings::GetPrompt(""),
      Strings::GetPrompt("title"),
      IostreamStrings::LeaveEmptyFor(default_date),
      Strings::GetPrompt("due date", kDatePattern),
      IostreamStrings::LeaveEmptyFor(Strings::to_string(Task::kLow)),
      IostreamStrings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      IostreamStrings::LeaveEmptyFor(
          Strings::to_string(Task::kUncompleted)),
      Strings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      Strings::ShowId(std::to_string(0)),
      Strings::GetPrompt(""),

      IostreamStrings::kAddSubtaskTo,
      Strings::ShowTask(TaskDataToTask(t1)),
      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(t1.date),
      Strings::GetPrompt("due date", kDatePattern),
      IostreamStrings::LeaveEmptyFor(t1.priority),
      IostreamStrings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      Strings::LeaveEmptyFor(t1.state),
      Strings::GetPrompt("state"),
      IostreamStrings::ProceedTo("add"),
      Strings::ShowId(std::to_string(1)),
      Strings::GetPrompt(""),

      Strings::kAddSubtaskTo,
      IostreamStrings::ShowTask(TaskDataToTask(sub_t1)),
      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(sub_t1.date),
      Strings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(sub_t1.priority),
      IostreamStrings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      Strings::LeaveEmptyFor(sub_t1.state),
      Strings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      Strings::ShowId(std::to_string(2)),

      Strings::GetPrompt(""),
      Strings::YouAreGoingTo("complete"),
      Strings::ShowTask(TaskDataToTask(t1)),
      Strings::AndItsChildren(std::to_string(
          task_storage.Find(CreateTaskId(0))->second.ShowStorage().size())),
      Strings::ProceedTo("complete"),

      Strings::GetPrompt(""),
  };

  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}
#endif