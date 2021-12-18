#include "IostreamStrings.h"

#include <google/protobuf/util/time_util.h>

#include <iomanip>
#include <sstream>

#include "repl/validator/DateFormat.h"

const char* IostreamStrings::to_string(Task::Priority priority) {
  if (priority == Task::kLow) {
    return "low";
  }
  if (priority == Task::kMedium) {
    return "medium";
  }
  if (priority == Task::kHigh) {
    return "high";
  }

  std::terminate();  // TODO: Handle buggy behavior
}

const char* IostreamStrings::to_string(Task::Progress state) {
  if (state == Task::kCompleted) {
    return "+";
  }
  if (state == Task::kUncompleted) {
    return "-";
  }

  std::terminate();  // TODO: Handle buggy behavior
}

std::string IostreamStrings::NotPresentId(const std::string& id) {
  std::stringstream ss;
  ss << "Id " << id << " not present in store.\n";
  return ss.str();
}

std::string IostreamStrings::ShowId(const std::string& id) {
  std::stringstream ss;
  ss << "Id " << id << ".\n";
  return ss.str();
}

std::string IostreamStrings::YouAreGoingTo(const std::string& str) {
  std::stringstream ss;
  ss << "You are going to " << str << " such tasks:\n";
  return ss.str();
}

std::string IostreamStrings::AndItsChildren(const std::string& n) {
  std::stringstream ss;
  ss << "  and its " << n << " children.\n";
  return ss.str();
}

std::string IostreamStrings::LeaveEmptyFor(const std::string& str) {
  std::stringstream ss;
  ss << "Leave empty for '" << str << "'\n";
  return ss.str();
}

std::string IostreamStrings::LeaveEmptyFor(const std::string& str1,
                                           const std::string& str2) {
  std::stringstream ss;
  ss << "Leave empty for '" << str1 << "'. " << str2 << "\n";
  return ss.str();
}

std::string IostreamStrings::GetPrompt(const std::string& str) {
  std::stringstream ss;
  ss << '[' << str << "]: ";
  return ss.str();
}

std::string IostreamStrings::GetPrompt(const std::string& str1,
                                       const std::string& str2) {
  std::stringstream ss;
  ss << '[' << str1 << "(" << str2 << ")]: ";
  return ss.str();
}

std::string IostreamStrings::ProceedTo(const std::string& str) {
  std::stringstream ss;
  ss << "Proceed to " << str << "? [Y/n]: ";
  return ss.str();
}

std::string IostreamStrings::InvalidId(const std::string& str) {
  std::stringstream ss;
  ss << "Value '" << str << "' expected to be numeric.\n";
  return ss.str();
}

std::string PrintAndGet(IIoFacility& io, const std::string& str) {
  io.Print(str);
  std::string input = io.GetLine();
  return input;
}

std::string IostreamStrings::ShowSolidTask(SolidTask solid_task) {
  std::stringstream ss;
  auto time = google::protobuf::util::TimeUtil::TimestampToTimeT(
      solid_task.task().due_date());
  auto localized_time = std::localtime(&time);
  ss << "└─ " << solid_task.task_id().id() << " - ["
     << to_string(solid_task.task().progress()) << "] "
     << "(" << to_string(solid_task.task().priority()) << ") "
     << "{" << std::put_time(localized_time, kDatePattern) << "} "
     << "'" << solid_task.task().title() << "'\n";
  return ss.str();
}

std::string IostreamStrings::ShowSolidTasks(SolidTasks solid_tasks) {
  std::stringstream ss;
  std::vector<TaskId> id_stack;
  for (const auto& i : solid_tasks) {
    if (id_stack.empty()) {
      if (i.has_parent_id()) {
        std::terminate();  // TODO: Ensure vector is sorted
      }
      id_stack.push_back(i.task_id());
      ss << ' ' << ShowSolidTask(i);
    } else {
      if (i.parent_id() == id_stack.back()) {
        ss << std::string((id_stack.size() + 1) * 2 - 1, ' ')
           << ShowSolidTask(i);
        id_stack.push_back(i.task_id());
      } else {
        for (; !id_stack.empty() && id_stack.back() != i.parent_id();) {
          id_stack.pop_back();
        }
        ss << std::string((id_stack.size() + 1) * 2 - 1, ' ')
           << ShowSolidTask(i);
        id_stack.push_back(i.task_id());
      }
    }
  }
  return ss.str();
}
