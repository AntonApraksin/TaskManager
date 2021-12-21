#include "UsageFramework.h"

class ShowAllOutputTest : public ::testing::Test, protected UsageFramework {
 protected:
  void SetUp() override { return SetUpImpl(); }
};

TEST_F(ShowAllOutputTest, RewriteItEveryTimeYouChangeTheSourceCode) {
  std::stringstream ss;
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&time), kDatePattern);

  std::string default_date = ss.str();
  auto t1 = task_stringed_data_producer_.GetData();
  auto sub_t1 = task_stringed_data_producer_.GetData();
  auto sub_sub_t1 = task_stringed_data_producer_.GetData();
  auto t2 = task_stringed_data_producer_.GetData();
  auto t3 = task_stringed_data_producer_.GetData();
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
                                             "a",
                                             t2.title,
                                             t2.date,
                                             t2.priority,
                                             t2.state,
                                             "y",
                                             "a",
                                             t3.title,
                                             t3.date,
                                             t3.priority,
                                             t3.state,
                                             "y",
                                             "s",
                                             "q"});
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
      Strings::ShowSolidTask(TaskDataToSolidTask(t1, 0)),
      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(t1.date),
      Strings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(t1.priority),
      Strings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      Strings::LeaveEmptyFor(t1.state),
      Strings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      Strings::ShowId(std::to_string(1)),
      Strings::GetPrompt(""),

      Strings::kAddSubtaskTo,
      Strings::ShowSolidTask(TaskDataToSolidTask(sub_t1, 1)),
      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(sub_t1.date),
      Strings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(sub_t1.priority),
      Strings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      Strings::LeaveEmptyFor(sub_t1.state),
      Strings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      Strings::ShowId(std::to_string(2)),

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
      Strings::ShowId(std::to_string(3)),

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
      Strings::ShowId(std::to_string(4)),
      Strings::GetPrompt(""),
      Strings::ShowSolidTasks(task_storage),
      Strings::GetPrompt(""),
  };

  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}