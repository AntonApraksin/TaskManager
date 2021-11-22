#include "AddNewREPLState.h"

#include "repl/context/Context.h"
#include "repl/state/MainREPLState.h"

IREPLState* AddNewREPLState::Execute(Context* ctx)
{
  auto printer = ctx->GetPrinter();
  printer->ChangePrompt("add");

  auto title = printer->AskForATitle();
  // validate
  auto date = printer->AskForADate();
  // validate
  auto priority = printer->AskForAPriority();
  // validate
  auto confirm = printer->AskForAConfirmation();
  // if confirm = y then else ...

  return new MainREPLState;
}