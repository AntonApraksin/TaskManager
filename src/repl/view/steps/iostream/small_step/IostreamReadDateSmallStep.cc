#include <iomanip>
#include <iostream>

#include "IostreamSmallStep.h"
#include "repl/validator/DateFormat.h"
#include "repl/view/steps/TaskContext.h"

// TODO: Prettify implementation

void IostreamReadDateSmallStep::Execute(TaskContext &ctx) {
  if (ctx.GetTaskBuilder().date_) {
    auto time =
        std::chrono::system_clock::to_time_t(*ctx.GetTaskBuilder().date_);
    std::cout << "Leave empty for '"
              << std::put_time(std::localtime(&time), kDatePattern) << "'\n";
  }
  std::cout << "[due date(" << kDatePattern << ")]: ";
  std::string date_string;
  std::getline(std::cin, date_string);
  if (date_string.empty()) {
    ctx.PopState();
    return;
  }
  auto validated_date = validator_->ParseTaskDate(date_string);
  for (; !validated_date;) {
    std::cout << "Wrong date format.\n";
    std::cout << "[due date(" << kDatePattern << ")]: ";
    std::getline(std::cin, date_string);
    if (date_string.empty()) {
      ctx.PopState();
      return;
    }
    validated_date = validator_->ParseTaskDate(date_string);
  }
  ctx.GetTaskBuilder().date_ = validated_date;
  ctx.PopState();
}