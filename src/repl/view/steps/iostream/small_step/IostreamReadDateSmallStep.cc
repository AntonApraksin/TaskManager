#include <iomanip>
#include <sstream>

#include "IostreamSmallStep.h"
#include "repl/validator/DateFormat.h"
#include "repl/view/steps/TaskContext.h"

// TODO: Prettify implementation

void IostreamReadDateSmallStep::Execute(TaskContext &ctx) {
  std::stringstream ss;
  if (ctx.GetTaskBuilder().date_) {
    auto time =
        std::chrono::system_clock::to_time_t(*ctx.GetTaskBuilder().date_);
    ss << "Leave empty for '"
              << std::put_time(std::localtime(&time), kDatePattern) << "'\n";
    io_facility_->Print(ss.str());
    ss.str("");
  }
  // TODO: Rewrite to PrintAndGet
  ss << "[due date(" << kDatePattern << ")]: ";
  io_facility_->Print(ss.str());
  ss.str("");
  std::string date_string = io_facility_->GetLine();
  if (date_string.empty()) {
    ctx.PopState();
    return;
  }
  auto validated_date = validator_->ParseTaskDate(date_string);
  for (; !validated_date;) {
    ss << "Wrong date format.\n"
    << "[due date(" << kDatePattern << ")]: ";
    io_facility_->Print(ss.str());
    ss.str(""); // TODO: avoid refilling
    date_string = io_facility_->GetLine();
    if (date_string.empty()) {
      ctx.PopState();
      return;
    }
    validated_date = validator_->ParseTaskDate(date_string);
  }
  ctx.GetTaskBuilder().date_ = validated_date;
  ctx.PopState();
}