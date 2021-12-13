#include "ScenarioFramework.h"

class ConfirmationNoTest : public testing::Test, protected ScenarioFramework {
 protected:
  void SetUp() override { return SetUpImpl(); }
};

TEST_F(ConfirmationNoTest, AddShouldNotBePerformed) {
  std::string title_1 = "938ugphurgskhk";

  auto storage = RunScenario({"add", title_1, "", "", "", "n", "q"});

  EXPECT_EQ(storage.ShowStorage().size(), 0);
}

TEST_F(ConfirmationNoTest, EditShouldNotBePerformed) {
  std::string title = "title";
  std::string date = "12:34 21/07/2021";
  std::string priority = "medium";
  std::string state = "+";

  auto storage =
      RunScenario({"add", title, date, priority, state, "y", "edit 0", "f",
                   "15:23 18/09/2077", "low", "-", "n", "q"});

  auto expected_task = Task::Create(
      title, *validator_->ParseTaskPriority(priority),
      *validator_->ParseTaskDate(date), *validator_->ParseTaskState(state));

  EXPECT_EQ(*storage.Find(TaskId::Create(0)), expected_task);
}

TEST_F(ConfirmationNoTest, CompleteShouldNotBePerformed) {
  std::string state = "-";

  auto storage =
      RunScenario({"add", "gdfgd", "", "", state, "y", "complete 0", "n", "q"});

  EXPECT_EQ(storage.Find(TaskId::Create(0))->GetState(),
            Task::State::kUncompleted);
}

TEST_F(ConfirmationNoTest, DeleteShouldNotBePerformed) {
  auto storage =
      RunScenario({"add", "gdfgd", "", "", "", "y", "delete 0", "n", "q"});

  EXPECT_NO_THROW(storage.Find(TaskId::Create(0)));  // NOLINT
}