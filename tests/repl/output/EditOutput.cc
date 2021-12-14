#include "UsageFramework.h"

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
      IostreamStrings::GetPrompt(""),
      IostreamStrings::GetPrompt("title"),
      IostreamStrings::LeaveEmptyFor(default_date),
      IostreamStrings::GetPrompt("due date", kDatePattern),
      IostreamStrings::LeaveEmptyFor(
          IostreamStrings::to_string(Task::Priority::kLow)),
      IostreamStrings::GetPrompt("priority"),
      IostreamStrings::kInvalidState,
      IostreamStrings::LeaveEmptyFor(
          IostreamStrings::to_string(Task::State::kUncompleted)),
      IostreamStrings::GetPrompt("state"),
      IostreamStrings::ProceedTo("add"),
      IostreamStrings::ShowId(0),

      IostreamStrings::GetPrompt(""),
      IostreamStrings::kAddSubtaskTo,
      IostreamStrings::ShowTask(TaskDataToTask(t1)),
      IostreamStrings::GetPrompt("title"),
      IostreamStrings::LeaveEmptyFor(t1.date),
      IostreamStrings::GetPrompt("due date", kDatePattern),
      IostreamStrings::LeaveEmptyFor(t1.priority),
      IostreamStrings::GetPrompt("priority"),
      IostreamStrings::kInvalidState,
      IostreamStrings::LeaveEmptyFor(t1.state),
      IostreamStrings::GetPrompt("state"),
      IostreamStrings::ProceedTo("add"),
      IostreamStrings::ShowId(1),

      IostreamStrings::GetPrompt(""),
      IostreamStrings::kYouAreGoingToEdit,
      IostreamStrings::ShowTask(TaskDataToTask(t1)),
      IostreamStrings::LeaveEmptyFor(t1.title),
      IostreamStrings::GetPrompt("title"),
      IostreamStrings::LeaveEmptyFor(t1.date),
      IostreamStrings::GetPrompt("due date", kDatePattern),
      IostreamStrings::LeaveEmptyFor(t1.priority),
      IostreamStrings::GetPrompt("priority"),
      IostreamStrings::kInvalidState,
      IostreamStrings::LeaveEmptyFor(t1.state),
      IostreamStrings::GetPrompt("state"),
      IostreamStrings::ProceedTo("edit"),

      IostreamStrings::GetPrompt(""),
      IostreamStrings::kYouAreGoingToEdit,
      IostreamStrings::ShowTask(TaskDataToTask(sub_t1)),
      IostreamStrings::LeaveEmptyFor(sub_t1.title),
      IostreamStrings::GetPrompt("title"),
      IostreamStrings::LeaveEmptyFor(sub_t1.date),
      IostreamStrings::GetPrompt("due date", kDatePattern),
      IostreamStrings::LeaveEmptyFor(sub_t1.priority),
      IostreamStrings::GetPrompt("priority"),
      IostreamStrings::kInvalidState,
      IostreamStrings::LeaveEmptyFor(sub_t1.state),
      IostreamStrings::GetPrompt("state"),
      IostreamStrings::ProceedTo("edit"),

      IostreamStrings::GetPrompt(""),
      IostreamStrings::kYouAreGoingToEdit,
      IostreamStrings::ShowTask(*task_storage.Find(TaskId::Create(0))),
      IostreamStrings::LeaveEmptyFor(edit_t1.title),
      IostreamStrings::GetPrompt("title"),
      IostreamStrings::LeaveEmptyFor(edit_t1.date),
      IostreamStrings::GetPrompt("due date", kDatePattern),
      IostreamStrings::LeaveEmptyFor(edit_t1.priority),
      IostreamStrings::GetPrompt("priority"),
      IostreamStrings::kInvalidState,
      IostreamStrings::LeaveEmptyFor(edit_t1.state),
      IostreamStrings::GetPrompt("state"),
      IostreamStrings::ProceedTo("edit"),

      IostreamStrings::GetPrompt(""),
  };

  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}