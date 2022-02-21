#include <gtest/gtest.h>

#include "test_utils/ScenarioFramework.h"

class PlainOutputTest : public ::testing::Test, protected ScenarioFramework
{
protected:
    void SetUp() override { return SetUpImpl(); }
};

TEST_F(PlainOutputTest, HelpMustAppear)
{
    auto[task_storage, output] = RunScenario({"h", "q"});

    std::vector<std::string> desired_output{
        Strings::GetPrompt(""),
        Strings::kHelp,
        Strings::GetPrompt(""),
    };

    ASSERT_EQ(output.size(), desired_output.size());

    for (size_t i{0}, sz{output.size()} ; i != sz ; ++i)
    {
        EXPECT_EQ(output[i], desired_output[i]);
    }
}
TEST_F(PlainOutputTest, InvalidIdMustAppear)
{
    auto[task_storage, output] = RunScenario({"a oqpw", "q"});

    std::vector<std::string> desired_output{
        Strings::GetPrompt(""),
        Strings::kInvalidId,
        Strings::GetPrompt(""),
    };

    ASSERT_EQ(output.size(), desired_output.size());

    for (size_t i{0}, sz{output.size()} ; i != sz ; ++i)
    {
        EXPECT_EQ(output[i], desired_output[i]);
    }
}

TEST_F(PlainOutputTest, MultipleIdMustAppear)
{
    std::stringstream ss;
    auto time =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    ss << std::put_time(std::localtime(&time), kDatePattern);
    std::string default_date = ss.str();

    auto[task_storage, output] =
    RunScenario({"a", "t", "", "", "", "y", "a 0 1", "q"});

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

        Strings::kMultipleArgumentDoesNotSupported,
        Strings::GetPrompt(""),
    };

    ASSERT_EQ(output.size(), desired_output.size());

    for (size_t i{0}, sz{output.size()} ; i != sz ; ++i)
    {
        EXPECT_EQ(output[i], desired_output[i]);
    }
}

TEST_F(PlainOutputTest, UnknownCommandMustAppear)
{
    std::stringstream ss;
    auto time =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    ss << std::put_time(std::localtime(&time), kDatePattern);
    std::string default_date = ss.str();

    auto[task_storage, output] = RunScenario({"sheesh", "q"});

    std::vector<std::string> desired_output{
        Strings::GetPrompt(""),
        Strings::kUnknownCommand,
        Strings::GetPrompt(""),
    };

    ASSERT_EQ(output.size(), desired_output.size());

    for (size_t i{0}, sz{output.size()} ; i != sz ; ++i)
    {
        EXPECT_EQ(output[i], desired_output[i]);
    }
}

TEST_F(PlainOutputTest, NotPresentIdMustAppear)
{
    std::stringstream ss;
    auto time =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    ss << std::put_time(std::localtime(&time), kDatePattern);
    std::string default_date = ss.str();

    auto[task_storage, output] =
    RunScenario({"e 0", "gds", "", "", "", "y", "q"});

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
        Strings::ProceedTo("edit"),
        Strings::kNotPresentId,
        Strings::GetPrompt(""),
    };

    ASSERT_EQ(output.size(), desired_output.size());

    for (size_t i{0}, sz{output.size()} ; i != sz ; ++i)
    {
        EXPECT_EQ(output[i], desired_output[i]);
    }
}

TEST_F(PlainOutputTest, RequiredIdMustAppear)
{
    std::stringstream ss;
    auto time =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    ss << std::put_time(std::localtime(&time), kDatePattern);
    std::string default_date = ss.str();

    auto[task_storage, output] = RunScenario({"e", "q"});

    std::vector<std::string> desired_output{
        Strings::GetPrompt(""),
        Strings::kRequiredId,
        Strings::GetPrompt(""),
    };

    ASSERT_EQ(output.size(), desired_output.size());

    for (size_t i{0}, sz{output.size()} ; i != sz ; ++i)
    {
        EXPECT_EQ(output[i], desired_output[i]);
    }
}