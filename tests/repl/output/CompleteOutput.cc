#include <gtest/gtest.h>

#include "test_utils/ScenarioFramework.h"

class CompleteOutputTest : public ::testing::Test, protected ScenarioFramework {
 protected:
  void SetUp() override { return SetUpImpl(); }
};

TEST_F(CompleteOutputTest, RewriteItEveryTimeYouChangeTheSourceCode) {
  std::stringstream ss;
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&time), kDatePattern);

  auto st0 = task_stringed_data_producer_.GetData();
  auto sub_st0 = task_stringed_data_producer_.GetData();
  auto sub_sub_st0 = task_stringed_data_producer_.GetData();
  auto [task_storage, output] = RunScenario({"a",
                                             st0.title,
                                             st0.date,
                                             st0.priority,
                                             st0.state,
                                             "y",
                                             "a 0",
                                             sub_st0.title,
                                             sub_st0.date,
                                             sub_st0.priority,
                                             sub_st0.state,
                                             "y",
                                             "a 1",
                                             sub_sub_st0.title,
                                             sub_sub_st0.date,
                                             sub_sub_st0.priority,
                                             sub_sub_st0.state,
                                             "y",
                                             "c 0",
                                             "y",
                                             "q"});
  auto storage_before_complete = task_storage;
  for (size_t i{0}, sz{storage_before_complete.size()}; i != sz; ++i) {
    auto new_task = storage_before_complete[i].task();
    new_task.set_progress(Task::kUncompleted);
    storage_before_complete[i].set_allocated_task(
        new Task(std::move(new_task)));
  }

  task_manager::SolidTask t0 = *std::find_if(
      storage_before_complete.cbegin(), storage_before_complete.cend(),
      [](const auto& i) { return i.task_id().id() == 0; });

  task_manager::SolidTask t1 = *std::find_if(
      storage_before_complete.cbegin(), storage_before_complete.cend(),
      [](const auto& i) { return i.task_id().id() == 1; });
  std::string default_date = ss.str();

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
      Strings::ShowSolidTask(t0),
      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(st0.date),
      Strings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(st0.priority),
      Strings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      Strings::LeaveEmptyFor(st0.state),
      Strings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      Strings::ShowId(std::to_string(1)),
      Strings::GetPrompt(""),

      Strings::kAddSubtaskTo,
      Strings::ShowSolidTask(t1),
      Strings::GetPrompt("title"),
      Strings::LeaveEmptyFor(sub_st0.date),
      Strings::GetPrompt("due date", kDatePattern),
      Strings::LeaveEmptyFor(sub_st0.priority),
      Strings::GetPrompt("priority"),
      Strings::kStateShouldBe,
      Strings::LeaveEmptyFor(sub_st0.state),
      Strings::GetPrompt("state"),
      Strings::ProceedTo("add"),
      Strings::ShowId(std::to_string(2)),

      Strings::GetPrompt(""),
      Strings::YouAreGoingTo("complete"),
      Strings::ShowSolidTasks(storage_before_complete),
      Strings::ProceedTo("complete"),

      Strings::GetPrompt(""),
  };

  ASSERT_EQ(output.size(), desired_output.size());

  for (size_t i{0}, sz{output.size()}; i != sz; ++i) {
    EXPECT_EQ(output[i], desired_output[i]);
  }
}