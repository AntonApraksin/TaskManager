#include "Strings.h"

#include <google/protobuf/util/time_util.h>

#include <iomanip>
#include <sstream>

#include "interactor/validator/DateFormat.h"
#include "utils/TaskIdUtils.h"

namespace task_manager {
const char* Strings::to_string(Task::Priority priority) {
  if (priority == Task::kLow) {
    return "low";
  }
  if (priority == Task::kMedium) {
    return "medium";
  }
  if (priority == Task::kHigh) {
    return "high";
  }

  std::terminate();  // Must be unreachable
}

const char* Strings::to_string(Task::Progress state) {
  if (state == Task::kCompleted) {
    return "+";
  }
  if (state == Task::kUncompleted) {
    return "-";
  }

  std::terminate();  // Must be unreachable
}

std::string Strings::NotPresentId(const std::string& id) {
  std::stringstream ss;
  ss << "Task '" << id << "' not present in store.\n";
  return ss.str();
}

std::string Strings::NotPresentLabel(const std::string& id,
                                     const std::string& label) {
  std::stringstream ss;
  ss << "Task '" << id << "' does not have label '" << label << "'.\n";
  return ss.str();
}

std::string Strings::ShowId(const std::string& id) {
  std::stringstream ss;
  ss << "Id " << id << ".\n";
  return ss.str();
}

std::string Strings::YouAreGoingTo(const std::string& str) {
  std::stringstream ss;
  ss << "You are going to " << str << " such tasks:\n";
  return ss.str();
}

std::string Strings::LeaveEmptyFor(const std::string& str) {
  std::stringstream ss;
  ss << "Leave empty for '" << str << "'\n";
  return ss.str();
}

std::string Strings::LeaveEmptyFor(const std::string& str1,
                                   const std::string& str2) {
  std::stringstream ss;
  ss << "Leave empty for '" << str1 << "'. " << str2 << "\n";
  return ss.str();
}

std::string Strings::GetPrompt(const std::string& str) {
  std::stringstream ss;
  ss << '[' << str << "]: ";
  return ss.str();
}

std::string Strings::GetPrompt(const std::string& str1,
                               const std::string& str2) {
  std::stringstream ss;
  ss << '[' << str1 << "(" << str2 << ")]: ";
  return ss.str();
}

std::string Strings::ProceedTo(const std::string& str) {
  std::stringstream ss;
  ss << "Proceed to " << str << "? [Y/n]: ";
  return ss.str();
}

std::string Strings::InvalidId(const std::string& str) {
  std::stringstream ss;
  ss << "Value '" << str << "' is expected to be numeric.\n";
  return ss.str();
}

std::string PrintAndGet(IIoFacility& io, const std::string& str) {
  io.Print(str);
  std::string input = io.GetLine();
  return input;
}

std::string Strings::ShowSolidTask(SolidTask solid_task) {
  std::stringstream ss;
  auto time = google::protobuf::util::TimeUtil::TimestampToTimeT(
      solid_task.task().due_date());
  auto localized_time = std::localtime(&time);
  ss << "└─ " << solid_task.task_id().id() << " - ["
     << to_string(solid_task.task().progress()) << "] "
     << "(" << to_string(solid_task.task().priority()) << ") "
     << "{" << std::put_time(localized_time, kDatePattern) << "} "
     << "'" << solid_task.task().title() << "' ";
  if (solid_task.task().labels_size() != 0) {
    ss << "( ";
    for (const auto& i : solid_task.task().labels()) {
      ss << i.name() << " ";
    }
    ss << ")";
  }
  ss << '\n';
  return ss.str();
}

std::string Strings::ShowSolidTasks(SolidTasks solid_tasks) {
  std::stringstream ss;
  std::vector<TaskId> id_stack;
  for (const auto& i : solid_tasks) {
    if (id_stack.empty()) {
      if (i.has_parent_id()) {
        std::terminate();  // Ensure vector is sorted
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

std::string Strings::IdIsSubIdOf(const std::string& parent,
                                 const std::string& child) {
  std::stringstream ss;
  ss << "Task '" << child << "' is a subtask of task '" << parent << "'.\n";
  return ss.str();
}
std::string Strings::FilenameDoesNotExist(const std::string& filename) {
  std::stringstream ss;
  ss << "File '" << filename << "' does not exists.\n";
  return ss.str();
}
std::string Strings::ErrorDuringOpeningFile(const std::string& filename) {
  std::stringstream ss;
  ss << "An error occurred when opening '" << filename << "'.\n";
  return ss.str();
}
std::string Strings::FailureDuringSaving(const std::string& filename) {
  std::stringstream ss;
  ss << "An error occurred while was saving to '" << filename << "'.\n";
  return ss.str();
}
std::string Strings::FailureDuringLoading(const std::string& filename) {
  std::stringstream ss;
  ss << "An error occurred while was loading from '" << filename << "'.\n";
  return ss.str();
}
}  // namespace task_manager
