#include <gtest/gtest.h>

#include "test_utils/ScenarioFramework.h"

class InvalidFieldsOutputTest : public ::testing::Test,
                                protected ScenarioFramework
{
protected:
    void SetUp() override { return SetUpImpl(); }
};

TEST_F(InvalidFieldsOutputTest, AddAction)
{
    std::stringstream ss;
    auto time =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    ss << std::put_time(std::localtime(&time), kDatePattern);
    std::string default_date = ss.str();
    auto[task_storage, output] =
    RunScenario({"a", /*title:*/ "", "", "title", /*date:*/ "12", "fsdf", "",
                    /*priority:*/ "inv", "sheesh", "",
                    /*state:*/ "rep", "1", "", "y", "q"});

    std::vector<std::string> desired_output{
        Strings::GetPrompt(""),
        Strings::GetPrompt("title"),
        Strings::kTitleMustNotBeEmpty,
        Strings::GetPrompt("title"),
        Strings::kTitleMustNotBeEmpty,
        Strings::GetPrompt("title"),

        Strings::LeaveEmptyFor(default_date),
        Strings::GetPrompt("due date", kDatePattern),
        Strings::kWrongDateFormat,
        Strings::GetPrompt("due date", kDatePattern),
        Strings::kWrongDateFormat,
        Strings::GetPrompt("due date", kDatePattern),

        Strings::LeaveEmptyFor(Strings::to_string(Task::kLow)),
        Strings::GetPrompt("priority"),
        Strings::kPriorityShouldBe,
        Strings::GetPrompt("priority"),
        Strings::kPriorityShouldBe,
        Strings::GetPrompt("priority"),

        Strings::kStateShouldBe,
        Strings::LeaveEmptyFor(Strings::to_string(Task::kUncompleted)),
        Strings::GetPrompt("state"),
        Strings::kStateShouldBe,
        Strings::GetPrompt("state"),
        Strings::kStateShouldBe,
        Strings::GetPrompt("state"),

        Strings::ProceedTo("add"),
        Strings::ShowId(std::to_string(0)),
        Strings::GetPrompt(""),
    };
    ASSERT_EQ(output.size(), desired_output.size());

    for (size_t i{0}, sz{output.size()} ; i != sz ; ++i)
    {
        EXPECT_EQ(output[i], desired_output[i]);
    }
}