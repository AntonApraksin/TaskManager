#include "model/task_manager/TaskManager.h"
#include "repl/Controller.h"
#include "repl/io_facility/iostream/IostreamIoFacility.h"
#include "repl/validator/DefaultValidator.h"
#include "repl/view/steps/iostream/small_step/IostreamSmallStepFactory.h"
#include "repl/view/steps/iostream/step/IostreamStepFactory.h"

int main() {
  auto validator = std::make_shared<DefaultValidator>();
  auto id_producer = std::make_unique<TaskIdProducer>();
  auto task_manager = std::make_shared<TaskManager>(std::move(id_producer));
  auto io_facility = std::make_shared<IostreamIoFacility>();
  auto state_factory =
      std::make_shared<IostreamSmallStepFactory>(io_facility, validator);
  auto step_factory = std::make_unique<IostreamStepFactory>(
      io_facility, state_factory, validator);
  Controller ctrl{io_facility, validator, task_manager,
                  std::move(step_factory)};
  ctrl.Run();

  return 0;
}