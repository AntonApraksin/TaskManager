#include "UsageFramework.h"

#if 0

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
                                             "title",
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
      IostreamStrings::ShowId(std::to_string(0)),
      IostreamStrings::GetPrompt(""),

      IostreamStrings::kAddSubtaskTo,
      IostreamStrings::ShowTask(*task_storage.Find(CreateTaskId(0))->second),
      IostreamStrings::GetPrompt("title"),
      IostreamStrings::LeaveEmptyFor(t1.date),
      IostreamStrings::GetPrompt("due date", kDatePattern),
      IostreamStrings::LeaveEmptyFor(t1.priority),
      IostreamStrings::GetPrompt("priority"),
      IostreamStrings::kInvalidState,
      IostreamStrings::LeaveEmptyFor(t1.state),
      IostreamStrings::GetPrompt("state"),
      IostreamStrings::ProceedTo("add"),
      IostreamStrings::ShowId(std::to_string(1)),
      IostreamStrings::GetPrompt(""),

      IostreamStrings::kAddSubtaskTo,
      IostreamStrings::ShowTask(*task_storage.Find(CreateTaskId(1))->second),
      IostreamStrings::GetPrompt("title"),
      IostreamStrings::LeaveEmptyFor(sub_t1.date),
      IostreamStrings::GetPrompt("due date", kDatePattern),
      IostreamStrings::LeaveEmptyFor(sub_t1.priority),
      IostreamStrings::GetPrompt("priority"),
      IostreamStrings::kInvalidState,
      IostreamStrings::LeaveEmptyFor(sub_t1.state),
      IostreamStrings::GetPrompt("state"),
      IostreamStrings::ProceedTo("add"),
      IostreamStrings::ShowId(std::to_string(2)),

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
      IostreamStrings::ShowId(std::to_string(3)),

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
      IostreamStrings::ShowId(std::to_string(4)),
      IostreamStrings::GetPrompt(""),
      IostreamStrings::ShowTaskWithId(
          *task_storage.Find(CreateTaskId(0))->second, CreateTaskId(0)),
      IostreamStrings::ShowNestedMap(task_storage.Find(CreateTaskId(0))->second,
                                     3),
      IostreamStrings::ShowTaskWithId(
          *task_storage.Find(CreateTaskId(3))->second, CreateTaskId(3)),
      IostreamStrings::ShowNestedMap(task_storage.Find(CreateTaskId(3))->second,
                                     3),
      IostreamStrings::ShowTaskWithId(
          *task_storage.Find(CreateTaskId(4))->second, CreateTaskId(4)),
      IostreamStrings::ShowNestedMap(task_storage.Find(CreateTaskId(4))->second,
                                     3),
      IostreamStrings::GetPrompt(""),
  };

  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}
#endif