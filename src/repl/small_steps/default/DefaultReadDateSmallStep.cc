#include <google/protobuf/util/time_util.h>

#include <iomanip>

#include "DefaultSmallStep.h"
#include "repl/io_facility/Strings.h"
#include "repl/small_steps/TaskContext.h"
#include "repl/validator/DateFormat.h"

namespace task_manager {
void DefaultReadDateSmallStep::Execute(TaskContext &ctx) {
  if (ctx.GetTaskBuilder().date_) {
    auto time = google::protobuf::util::TimeUtil::TimestampToTimeT(
        *ctx.GetTaskBuilder().date_);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), kDatePattern);
    std::string leave_empty_for = Strings::LeaveEmptyFor(ss.str());
    io_facility_->Print(leave_empty_for);
  }
  std::string prompt = Strings::GetPrompt("due date", kDatePattern);
  std::string date_string = PrintAndGet(*io_facility_, prompt);
  if (date_string.empty()) {
    ctx.PopState();
    return;
  }
  auto validated_date = validator_->ParseTaskDate(date_string);
  for (; !validated_date;) {
    io_facility_->Print(Strings::kWrongDateFormat);
    date_string = PrintAndGet(*io_facility_, prompt);
    if (date_string.empty()) {
      ctx.PopState();
      return;
    }
    validated_date = validator_->ParseTaskDate(date_string);
  }
  ctx.GetTaskBuilder().date_ = validated_date;
  ctx.PopState();
}
}  // namespace task_manager