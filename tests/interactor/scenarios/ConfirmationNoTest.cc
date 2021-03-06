#include <gtest/gtest.h>

#include "test_utils/ScenarioFramework.h"
#include "test_utils/operators.h"
#include "test_utils/utils.h"

class ConfirmationNoTest : public testing::Test, protected ScenarioFramework
{
protected:
    void SetUp() override { return SetUpImpl(); }
};

TEST_F(ConfirmationNoTest, AddShouldNotBePerformed)
{
    std::string title_1 = "938ugphurgskhk";

    auto[storage, _] = RunScenario({"add", title_1, "", "", "", "n", "q"});

    EXPECT_EQ(storage.size(), 0);
}

TEST_F(ConfirmationNoTest, EditShouldNotBePerformed)
{
    std::string title = "title";
    std::string date = "12:34 21/07/2021";
    std::string priority = "medium";
    std::string progress = "+";

    auto[storage, _] =
    RunScenario({"add", title, date, priority, progress, "y", "edit 0", "f",
                 "15:23 18/09/2077", "low", "-", "n", "q"});

    auto expected_task = CreateTask(title, *validator_->ParseTaskDate(date),
                                    *validator_->ParseTaskPriority(priority),
                                    *validator_->ParseTaskProgress(progress));

    EXPECT_EQ(FindSolidTask(storage, 0).task(), expected_task);
}

TEST_F(ConfirmationNoTest, CompleteShouldNotBePerformed)
{
    std::string state = "-";

    auto[storage, _] =
    RunScenario({"add", "gdfgd", "", "", state, "y", "complete 0", "n", "q"});

    EXPECT_EQ(FindSolidTask(storage, 0).task().progress(), Task::kUncompleted);
}

TEST_F(ConfirmationNoTest, DeleteShouldNotBePerformed)
{
    auto[storage, _] =
    RunScenario({"add", "gdfgd", "", "", "", "y", "delete 0", "n", "q"});

    ASSERT_EQ(storage.size(), 1);
}