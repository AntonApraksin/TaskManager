#include "model/task_manager/TaskManager.h"
#include "repl/Controller.h"
#include "repl/io_facility/DefaultValidator.h"
#include "repl/view/small_step/iostream_small_step/IostreamSmallStepFactory.h"
#include "repl/view/step/iostream_step/IostreamStepFactory.h"

int main() {
  auto validator = std::make_shared<DefaultValidator>();
  auto id_producer = std::make_unique<TaskIdProducer>();
  auto task_manager = std::make_shared<TaskManager>(std::move(id_producer));
  auto state_factory = std::make_shared<IostreamSmallStepFactory>(validator);
  auto step_factory =
      std::make_unique<IostreamStepFactory>(state_factory, validator);
  Controller ctrl{validator, task_manager, std::move(step_factory)};
  ctrl.Run();

  return 0;
}