#include "UsageFramework.h"

#if 0

class EditOutputTest : public ::testing::Test, protected UsageFramework {
 protected:
  void SetUp() override { return SetUpImpl(); }
};

TEST_F(EditOutputTest, RewriteItEveryTimeYouChangeTheSourceCode) {
  std::stringstream ss;
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&time), kDatePattern);

  std::string default_date = ss.str();
  auto t1 = task_stringed_data_producer_.GetData();
  auto edit_t1 = task_stringed_data_producer_.GetData();
  auto sub_t1 = task_stringed_data_producer_.GetData();
  auto edit_sub_t1 = task_stringed_data_producer_.GetData();

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
                                             "e 0",
                                             edit_t1.title,
                                             edit_t1.date,
                                             edit_t1.priority,
                                             edit_t1.state,
                                             "y",
                                             "e 1",
                                             edit_sub_t1.title,
                                             edit_sub_t1.date,
                                             edit_sub_t1.priority,
                                             edit_sub_t1.state,
                                             "y",
                                             "e 0",
                                             "",
                                             "",
                                             "",
                                             "",
                                             "y",
                                             "q"});

  std::vector<std::string> desired_output{
      Strings::GetPrompt(""),
      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(default_date),
      IostreamStrings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(Strings::to_string(Task::kLow)),
      Strings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      Strings::LeaveEmptyFor(
          Strings::to_string(Task::kUncompleted)),
      Strings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      Strings::ShowId(std::to_string(0)),

      Strings::GetPrompt(""),
      Strings::kAddSubtaskTo,
      Strings::ShowTask(TaskDataToTask(t1)),
      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(t1.date),
      IostreamStrings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(t1.priority),
      IostreamStrings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      Strings::LeaveEmptyFor(t1.state),
      IostreamStrings::GetPrompt("state"),
      IostreamStrings::ProceedTo("add"),
      Strings::ShowId(std::to_string(1)),

      Strings::GetPrompt(""),
      Strings::kYouAreGoingToEdit,
      Strings::ShowTask(TaskDataToTask(t1)),
      Strings::LeaveEmptyFor(t1.title),
      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(t1.date),
      Strings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(t1.priority),
      Strings::GetPrompt("priority"),
      IostreamStrings::kStateShouldBe,
      Strings::LeaveEmptyFor(t1.state),
      IostreamStrings::GetPrompt("state"),
      Strings::ProceedTo("edit"),

      IostreamStrings::GetPrompt(""),
      Strings::kYouAreGoingToEdit,
      IostreamStrings::ShowTask(TaskDataToTask(sub_t1)),
      IostreamStrings::LeaveEmptyFor(sub_t1.title),
      IostreamStrings::GetPrompt("title"),
      IostreamStrings::LeaveEmptyFor(sub_t1.date),
      IostreamStrings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(sub_t1.priority),
      IostreamStrings::GetPrompt("priority"),
      IostreamStrings::kStateShouldBe,
      IostreamStrings::LeaveEmptyFor(sub_t1.state),
      IostreamStrings::GetPrompt("state"),
      Strings::ProceedTo("edit"),

      Strings::GetPrompt(""),
      IostreamStrings::kYouAreGoingToEdit,
      IostreamStrings::ShowTask(*task_storage.Find(CreateTaskId(0))->second),
      IostreamStrings::LeaveEmptyFor(edit_t1.title),
      Strings::GetPrompt("title"),
      IostreamStrings::LeaveEmptyFor(edit_t1.date),
      IostreamStrings::GetPrompt("due date", kDatePattern),
      IostreamStrings::LeaveEmptyFor(edit_t1.priority),
      Strings::GetPrompt("priority"),
      IostreamStrings::kStateShouldBe,
      IostreamStrings::LeaveEmptyFor(edit_t1.state),
      IostreamStrings::GetPrompt("state"),
      IostreamStrings::ProceedTo("edit"),

      Strings::GetPrompt(""),
  };

  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}
#endif