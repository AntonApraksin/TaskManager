#include <sstream>

#include "IostreamSmallStep.h"
#include "repl/view/steps/TaskContext.h"
#include "repl/view/steps/iostream/IostreamGeneralFunctional.h"

// TODO: Prettify implementation

void IostreamReadTitleSmallStep::Execute(TaskContext &ctx) {
  std::stringstream ss;
  if (ctx.GetTaskBuilder().title_) {
    ss << "Leave empty for '" << *ctx.GetTaskBuilder().title_ << "'\n";
    io_facility_->Print(ss.str());
    ss.str("");
    auto title = validator_->ValidateTitle(PrintAndGet(*io_facility_, "title"));
    if (title) {
      ctx.GetTaskBuilder().title_ = std::move(title);
      ctx.PopState();
      return;
    } else {
      ctx.PopState();
      return;
    }
  }
  auto title = validator_->ValidateTitle(PrintAndGet(*io_facility_, "title"));
  for (; !title;) {
    ss << "Title must not be empty.\n";
    io_facility_->Print(ss.str());
    ss.str("");
    title = validator_->ValidateTitle(PrintAndGet(*io_facility_, "title"));
  }
  ctx.GetTaskBuilder().title_ = std::move(title);
  ctx.PopState();
}