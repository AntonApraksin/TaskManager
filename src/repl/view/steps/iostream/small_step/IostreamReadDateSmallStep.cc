#include <iomanip>

#include "IostreamSmallStep.h"
#include "repl/validator/DateFormat.h"
#include "repl/view/steps/TaskContext.h"
#include "repl/view/steps/iostream/IostreamStrings.h"

// TODO: Prettify implementation

void IostreamReadDateSmallStep::Execute(TaskContext &ctx) {
  if (ctx.GetTaskBuilder().date_) {
    auto time =
        std::chrono::system_clock::to_time_t(*ctx.GetTaskBuilder().date_);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), kDatePattern);
    std::string leave_empty_for = IostreamStrings::LeaveEmptyFor(ss.str());
    io_facility_->Print(leave_empty_for);
  }
  // TODO: Rewrite to PrintAndGet
  std::string prompt = IostreamStrings::GetPrompt("due date", kDatePattern);
  std::string date_string = PrintAndGet(*io_facility_, prompt);
  if (date_string.empty()) {
    ctx.PopState();
    return;
  }
  auto validated_date = validator_->ParseTaskDate(date_string);
  for (; !validated_date;) {
    io_facility_->Print(IostreamStrings::kInvalidDate);
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