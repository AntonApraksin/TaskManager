#include "DefaultValidator.h"

#include <google/protobuf/util/time_util.h>

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <regex>  // TODO: Avoid regex
#include <sstream>

#include "interactor/validator/DateFormat.h"

namespace task_manager
{
void lower_string(std::string& str)
{
    std::transform(str.cbegin(), str.cend(), str.begin(),
                   [](auto c) { return std::tolower(c); });
}

CommandEnum DefaultValidator::MatchCommand(const std::string& str)
{
    if (str == "a" || str == "add")
    {
        return CommandEnum::kAdd;
    }
    if (str == "q" || str == "quit")
    {
        return CommandEnum::kQuit;
    }
    if (str == "h" || str == "help")
    {
        return CommandEnum::kHelp;
    }
    if (str == "d" || str == "delete")
    {
        return CommandEnum::kDelete;
    }
    if (str == "c" || str == "complete")
    {
        return CommandEnum::kComplete;
    }
    if (str == "e" || str == "edit")
    {
        return CommandEnum::kEdit;
    }
    if (str == "s" || str == "show")
    {
        return CommandEnum::kShow;
    }
    if (str == "save" || str == "sa")
    {
        return CommandEnum::kSave;
    }
    if (str == "load" || str == "lo")
    {
        return CommandEnum::kLoad;
    }

    if (str == "add_label" || str == "al")
    {
        return CommandEnum::kAddLabel;
    }

    if (str == "delete_label" || str == "dl")
    {
        return CommandEnum::kDeleteLabel;
    }
    return CommandEnum::kUnknown;
}
std::pair<CommandEnum, std::string> DefaultValidator::MakeRequest(
    std::string str)
{
    lower_string(str);
    auto command = MatchCommand(ConsumeOneTokenFrom(str));
    return {command, str};
}

std::optional<Task::Priority> DefaultValidator::ParseTaskPriority(
    const std::string& str)
{
    std::string input(str);
    lower_string(input);

    if (input == "high")
    {
        return Task::kHigh;
    }
    if (input == "medium")
    {
        return Task::kMedium;
    }
    if (input == "low")
    {
        return Task::kLow;
    }
    return std::nullopt;
}

std::optional<Task::Progress> DefaultValidator::ParseTaskProgress(
    const std::string& str)
{
    std::string input(str);
    lower_string(input);
    if (str == "+")
    {
        return Task::kCompleted;
    }
    if (str == "-")
    {
        return Task::kUncompleted;
    }
    return std::nullopt;
}

std::optional<TaskDate_t> DefaultValidator::ParseTaskDate(
    const std::string& str)
{
    std::tm tm{};
    std::regex pattern_regex(kDateRegex);
    if (!std::regex_match(str, pattern_regex))
    {
        return std::nullopt;
    }
    std::istringstream ss(str);
    ss >> std::get_time(&tm, kDatePattern);
    if (ss.fail())
    {
        return std::nullopt;
    }
    if (tm.tm_hour >= 12)
    {
        tm.tm_isdst = -1;  // TODO: FIX THIS. SUMMER/WINTER TIME SADNESS
    }
    else
    {
        tm.tm_isdst = -1;  // TODO: FIX THIS. SUMMER/WINTER TIME SADNESS
    }
    auto tp =
        google::protobuf::util::TimeUtil::TimeTToTimestamp(std::mktime(&tm));
    return tp;
}

std::optional<ConfirmationResult> DefaultValidator::ParseConfirmation(
    const std::string& str)
{
    std::string input(str);
    lower_string(input);
    if (str == "y" || str == "yes")
    {
        return ConfirmationResult::kYes;
    }
    if (str == "n" || str == "no")
    {
        return ConfirmationResult::kNo;
    }
    return std::nullopt;
}

std::optional<std::string> DefaultValidator::ValidateTitle(
    const std::string& str)
{
    if (str.empty())
    {
        return std::nullopt;
    }
    else
    {
        return str;
    }
}

std::string DefaultValidator::ConsumeOneTokenFrom(std::string& str)
{
    auto pos = str.find(' ');
    if (pos == std::string::npos)
    {
        // TODO: Handle error
        auto to_return = str;
        str = "";
        return to_return;
    }
    auto to_return = str.substr(0, pos);
    str.erase(0, pos + 1);
    return to_return;
}

std::optional<int> DefaultValidator::ParseInt(const std::string& str)
{
    try
    {
        return std::stoi(str);
    } catch (std::invalid_argument)
    {
        return {};
    }
}
}  // namespace task_manager