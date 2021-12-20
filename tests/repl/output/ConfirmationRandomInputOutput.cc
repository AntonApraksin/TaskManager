#include "UsageFramework.h"

#if 0

class ConfirmationRandomInputOutputTest : public ::testing::Test,
                                          protected UsageFramework {
 protected:
  void SetUp() override { return SetUpImpl(); }
};

TEST_F(ConfirmationRandomInputOutputTest, AddRandomInputMustCancel) {
  std::string date = "11:38 17/09/2019";
  std::stringstream ss;
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&time), kDatePattern);
  std::string default_date = ss.str();
  auto [task_storage, output] =
      RunScenario({"a", "title", date, "", "", "ui", "q"});

  std::vector<std::string> desired_output{
      Strings::GetPrompt(""),
      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(default_date),
      Strings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(Strings::to_string(Task::kLow)),
      Strings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      Strings::LeaveEmptyFor(
          Strings::to_string(Task::kUncompleted)),
      Strings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      Strings::kOkayITreatItAsNo,
      Strings::GetPrompt(""),
  };
  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}

TEST_F(ConfirmationRandomInputOutputTest, EditRandomInputMustCancel) {
  std::stringstream ss;
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&time), kDatePattern);

  std::string default_date = ss.str();
  auto t1 = task_stringed_data_producer_.GetData();
  auto edit_t1 = task_stringed_data_producer_.GetData();

  auto [task_storage, output] = RunScenario(
      {"a", t1.title, t1.date, t1.priority, t1.state, "y", "e 0", edit_t1.title,
       edit_t1.date, edit_t1.priority, edit_t1.state, "sheesh", "q"});

  std::vector<std::string> desired_output{
      Strings::GetPrompt(""),
      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(default_date),
      Strings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(Strings::to_string(Task::kLow)),
      Strings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      Strings::LeaveEmptyFor(
          Strings::to_string(Task::kUncompleted)),
      Strings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      Strings::ShowId(std::to_string(0)),

      Strings::GetPrompt(""),
      Strings::kYouAreGoingToEdit,
      Strings::ShowTask(TaskDataToTask(t1)),
      Strings::LeaveEmptyFor(t1.title),
      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(t1.date),
      Strings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(t1.priority),
      Strings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      Strings::LeaveEmptyFor(t1.state),
      Strings::GetPrompt("state"),
      Strings::ProceedTo("edit"),
      Strings::kOkayITreatItAsNo,

      Strings::GetPrompt(""),
  };

  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}

TEST_F(ConfirmationRandomInputOutputTest, CompleteRandomInputMustCancel) {
  std::stringstream ss;
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&time), kDatePattern);

  std::string default_date = ss.str();
  auto t1 = task_stringed_data_producer_.GetData();
  auto edit_t1 = task_stringed_data_producer_.GetData();

  auto [task_storage, output] =
      RunScenario({"a", t1.title, t1.date, t1.priority, t1.state, "y", "c 0",
                   "sheesh", "q"});

  std::vector<std::string> desired_output{
      Strings::GetPrompt(""),
      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(default_date),
      Strings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(Strings::to_string(Task::kLow)),
      Strings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      Strings::LeaveEmptyFor(
          Strings::to_string(Task::kUncompleted)),
      Strings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      Strings::ShowId(std::to_string(0)),

      Strings::GetPrompt(""),
      Strings::YouAreGoingTo("complete"),
      Strings::ShowTask(TaskDataToTask(t1)),
      Strings::AndItsChildren(std::to_string(0)),
      Strings::ProceedTo("complete"),
      Strings::kOkayITreatItAsNo,

      Strings::GetPrompt(""),
  };

  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}

TEST_F(ConfirmationRandomInputOutputTest, DeleteRandomInputMustCancel) {
  std::stringstream ss;
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&time), kDatePattern);

  std::string default_date = ss.str();
  auto t1 = task_stringed_data_producer_.GetData();
  auto edit_t1 = task_stringed_data_producer_.GetData();

  auto [task_storage, output] =
      RunScenario({"a", t1.title, t1.date, t1.priority, t1.state, "y", "d 0",
                   "sheesh", "q"});

  std::vector<std::string> desired_output{
      Strings::GetPrompt(""),
      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(default_date),
      Strings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(Strings::to_string(Task::kLow)),
      Strings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      Strings::LeaveEmptyFor(
          Strings::to_string(Task::kUncompleted)),
      Strings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      Strings::ShowId(std::to_string(0)),

      Strings::GetPrompt(""),
      Strings::YouAreGoingTo("delete"),
      Strings::ShowTask(TaskDataToTask(t1)),
      Strings::AndItsChildren(std::to_string(0)),
      Strings::ProceedTo("delete"),
      Strings::kOkayITreatItAsNo,

      Strings::GetPrompt(""),
  };

  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}
#endif