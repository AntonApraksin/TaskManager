#include <gtest/gtest.h>

#include "test_utils/ScenarioFramework.h"

class AddLabelOutputTest : public ::testing::Test, protected ScenarioFramework
{
protected:
    void SetUp() override { return SetUpImpl(); }
};

TEST_F(AddLabelOutputTest, AddLabelShowTasksAndDeleteLabelShowTasks)
{
    std::stringstream ss;
    auto time =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    ss << std::put_time(std::localtime(&time), kDatePattern);
    std::string default_date = ss.str();

    auto stringed_task = task_stringed_data_producer_.GetData();
    auto task = TaskDataToSolidTask(stringed_task, 0);
    auto task_with_label = task;
    {
        auto label = task_with_label.mutable_task()->add_labels();
        label->set_name("foo");
    }
    auto[task_storage, output] = RunScenario(
        {"a", stringed_task.title, stringed_task.date, stringed_task.priority,
         stringed_task.state, "y", "add_label 0 foo", "y", "s",
         "delete_label 0 foo", "y", "s", "q"});

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
        Strings::ProceedTo("add label"),

        Strings::GetPrompt(""),
        Strings::ShowSolidTasks({task_with_label}),

        Strings::GetPrompt(""),
        Strings::YouAreGoingTo("delete label from"),
        Strings::ShowSolidTask(task_with_label),
        Strings::ProceedTo("delete label"),

        Strings::GetPrompt(""),
        Strings::ShowSolidTasks({task}),

        Strings::GetPrompt(""),

    };
    ASSERT_EQ(output.size(), desired_output.size());

    for (size_t i{0}, sz{output.size()} ; i != sz ; ++i)
    {
        EXPECT_EQ(output[i], desired_output[i]);
    }
}

TEST_F(AddLabelOutputTest, AddLabelToNotPresentId)
{
    auto[task_storage, output] = RunScenario({"add_label 0 foo", "y", "q"});

    std::vector<std::string> desired_output{
        Strings::GetPrompt(""),
        Strings::ProceedTo("add label"),
        Strings::kNotPresentId,
        Strings::GetPrompt(""),
    };

    ASSERT_EQ(output.size(), desired_output.size());
    for (size_t i{0}, sz{output.size()} ; i != sz ; ++i)
    {
        EXPECT_EQ(output[i], desired_output[i]);
    }
}

TEST_F(AddLabelOutputTest, RandomConfirmation)
{
    std::stringstream ss;
    auto time =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    ss << std::put_time(std::localtime(&time), kDatePattern);
    std::string default_date = ss.str();

    auto stringed_task = task_stringed_data_producer_.GetData();
    auto task = TaskDataToSolidTask(stringed_task, 0);
    auto task_with_label = task;
    {
        auto label = task_with_label.mutable_task()->add_labels();
        label->set_name("foo");
    }
    auto[task_storage, output] = RunScenario(
        {"a", stringed_task.title, stringed_task.date, stringed_task.priority,
         stringed_task.state, "y", "add_label 0 foo", "fla", "q"});

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
        Strings::ProceedTo("add label"),
        Strings::kOkayITreatItAsNo,

        Strings::GetPrompt(""),
    };
    ASSERT_EQ(output.size(), desired_output.size());

    for (size_t i{0}, sz{output.size()} ; i != sz ; ++i)
    {
        EXPECT_EQ(output[i], desired_output[i]);
    }
}
