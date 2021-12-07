#include "model/task_manager/TaskManager.h"
#include "repl/Controller.h"
#include "repl/io_facility/iostream/IostreamIoFacility.h"
#include "repl/validator/DefaultValidator.h"
#include "repl/view/steps/iostream/small_step/IostreamSmallStepFactory.h"
#include "repl/view/steps/iostream/step/IostreamStepFactory.h"

int main() {
  auto id_producer = std::make_unique<TaskIdProducer>();
  auto task_manager = std::make_shared<TaskManager>(std::move(id_producer));

  auto validator = std::make_shared<DefaultValidator>();
  auto io_facility = std::make_shared<IostreamIoFacility>();

  auto small_step_factory =
      std::make_shared<IostreamSmallStepFactory>(io_facility, validator);
  auto step_factory = std::make_unique<IostreamStepFactory>(
      io_facility, validator, small_step_factory);

  auto view = std::make_unique<View>(io_facility, validator);
  Controller ctrl{std::move(view), task_manager, std::move(step_factory)};
  ctrl.Run();

  return 0;
}