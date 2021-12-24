#include "model/ModelController.h"
#include "model/task_manager/TaskManager.h"
#include "persistence/Persistence.h"
#include "repl/UIController.h"
#include "repl/io_facility/iostream/IostreamIoFacility.h"
#include "repl/validator/DefaultValidator.h"
#include "repl/view/steps/default/small_step/DefaultSmallStepFactory.h"
#include "repl/view/steps/default/step/DefaultStepFactory.h"

int main() {
  using namespace task_manager;
  auto id_producer = std::make_unique<TaskIdProducer>();
  auto task_manager = std::make_unique<TaskManager>(std::move(id_producer));

  auto validator = std::make_shared<DefaultValidator>();
  auto io_facility = std::make_shared<IostreamIoFacility>();

  auto small_step_factory =
      std::make_shared<DefaultSmallStepFactory>(io_facility, validator);
  auto step_factory = std::make_unique<DefaultStepFactory>(
      io_facility, validator, small_step_factory);

  auto persistence = std::make_unique<Persistence>();

  auto view = std::make_unique<View>(io_facility, validator);
  auto model_controller =
      std::make_shared<ModelController>(std::move(task_manager), std::move(persistence));
  UIController ctrl{std::move(view), model_controller, validator,
                    std::move(step_factory)};
  ctrl.Run();

  return 0;
}