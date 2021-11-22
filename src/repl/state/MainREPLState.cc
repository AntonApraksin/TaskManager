#include "MainREPLState.h"

#include "repl/context/Context.h"
#include "repl/state/AddNewREPLState.h"

#include <iostream>

IREPLState* MainREPLState::Execute(Context* ctx)
{
  auto printer = ctx->GetPrinter();
  printer->ChangePrompt("");
  auto action = printer->AskForAnAction();
  if (action == "add")
  {
    return new AddNewREPLState;
  }
  if (action == "exit")
  {
    return nullptr;
  }
  std::terminate();
}