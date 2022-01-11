#include <gtest/gtest.h>

#include "test_utils/ScenarioFramework.h"

class ConfirmationRandomInputOutputTest : public ::testing::Test,
                                          protected ScenarioFramework {
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
      Strings::LeaveEmptyFor(Strings::to_string(Task::kUncompleted)),
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
  auto st0 = task_stringed_data_producer_.GetData();
  auto edit_st0 = task_stringed_data_producer_.GetData();

  auto [task_storage, output] =
      RunScenario({"a", st0.title, st0.date, st0.priority, st0.state, "y",
                   "e 0", edit_st0.title, edit_st0.date, edit_st0.priority,
                   edit_st0.state, "sheesh", "q"});

  SolidTask t0 = TaskDataToSolidTask(st0, 0);
  SolidTask edit_t0 = TaskDataToSolidTask(edit_st0, 0);

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
      Strings::kYouAreGoingToEdit,
      Strings::ShowSolidTask(t0),
      Strings::LeaveEmptyFor(st0.title),
      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(st0.date),
      Strings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(st0.priority),
      Strings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      Strings::LeaveEmptyFor(st0.state),
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
      Strings::LeaveEmptyFor(Strings::to_string(Task::kUncompleted)),
      Strings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      Strings::ShowId(std::to_string(0)),

      Strings::GetPrompt(""),
      Strings::YouAreGoingTo("complete"),
      Strings::ShowSolidTasks(task_storage),
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
      Strings::LeaveEmptyFor(Strings::to_string(Task::kUncompleted)),
      Strings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      Strings::ShowId(std::to_string(0)),

      Strings::GetPrompt(""),
      Strings::YouAreGoingTo("delete"),
      Strings::ShowSolidTasks(task_storage),
      Strings::ProceedTo("delete"),
      Strings::kOkayITreatItAsNo,

      Strings::GetPrompt(""),
  };

  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}