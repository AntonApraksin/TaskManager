#include "model/task_manager/TaskManager.h"
#include "repl/Controller.h"
#include "repl/io_facility/DefaultIOFacility.h"
#include "repl/io_facility/DefaultValidator.h"
#include "repl/state_factory/DefaultStateFactory.h"
#include "repl/view/step/iostream_step/IostreamStepFactory.h"

int main() {
  auto printer = std::make_shared<DefaultIOFacility>();
  auto validator = std::make_shared<DefaultValidator>();
  auto id_producer = std::make_unique<TaskIdProducer>();
  auto task_manager = std::make_shared<TaskManager>(std::move(id_producer));
  auto state_factory =
      std::make_shared<DefaultStateFactory>(printer, validator);
  auto step_factory = std::make_unique<IostreamStepFactory>(state_factory, validator);
  Controller ctrl{printer, validator, task_manager, std::move(step_factory)};
  ctrl.Run();

  return 0;
}