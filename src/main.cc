#include "repl/context/Context.h"
#include "repl/print/DefaultPrinter.h"
#include "repl/print/DefaultValidator.h"
#include "repl/state_factory/DefaultStateFactory.h"

int main() {
  auto printer = std::make_shared<DefaultPrinter>();
  auto validator = std::make_shared<DefaultValidator>();
  auto state_factory =
      std::make_unique<DefaultStateFactory>(printer, validator);
  Context ctx(std::move(state_factory));
  ctx.Run();

  return 0;
}