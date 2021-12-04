#include <iostream>

#include "IostreamSmallStep.h"
#include "repl/view/steps/TaskContext.h"
#include "repl/view/steps/iostream/IostreamGeneralFunctional.h"

// TODO: Prettify implementation

void IostreamReadTitleSmallStep::Execute(TaskContext &ctx) {
  if (ctx.GetTaskBuilder().title_) {
    std::cout << "Leave empty for '" << *ctx.GetTaskBuilder().title_ << "'\n";
    auto title = validator_->ValidateTitle(PrintAndGet("title"));
    if (title) {
      ctx.GetTaskBuilder().title_ = std::move(title);
      ctx.PopState();
      return;
    } else {
      ctx.PopState();
      return;
    }
  }
  auto title = validator_->ValidateTitle(PrintAndGet("title"));
  for (; !title;) {
    std::cout << "Title must not be empty.\n";
    title = validator_->ValidateTitle(PrintAndGet("title"));
  }
  ctx.GetTaskBuilder().title_ = std::move(title);
  ctx.PopState();
}