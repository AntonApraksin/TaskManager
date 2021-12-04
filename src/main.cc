#include "model/task_manager/TaskManager.h"
#include "repl/Controller.h"
#include "repl/validator/DefaultValidator.h"
#include "repl/view/steps/iostream/small_step/IostreamSmallStepFactory.h"
#include "repl/view/steps/iostream/step/IostreamStepFactory.h"

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