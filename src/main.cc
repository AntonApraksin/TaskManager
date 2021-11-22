#include "repl/context/Context.h"
#include "repl/state/MainREPLState.h"
#include "repl/print/DefaultPrinter.h"

int main()
{
  auto state = std::make_shared<MainREPLState>();
  auto printer = std::make_shared<DefaultPrinter>();
  Context ctx(std::move(state), std::move(printer));
  ctx.Run();

  return 0;
}