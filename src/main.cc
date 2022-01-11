#include "model/DefaultModelController.h"
#include "model/task_manager/TaskManager.h"
#include "persistence/Persistence.h"
#include "repl/io_facility/iostream/IostreamIoFacility.h"
#include "repl/small_steps/default/DefaultSmallStepFactory.h"
#include "repl/state_machine/StateMachineController.h"
#include "repl/validator/DefaultValidator.h"

int main() {
  using namespace task_manager;
  auto id_producer = std::make_unique<TaskIdProducer>();
  auto task_manager = std::make_unique<TaskManager>(std::move(id_producer));

  auto validator = std::make_shared<DefaultValidator>();
  auto io_facility = std::make_shared<IostreamIoFacility>();
  auto small_step_factory =
      std::make_shared<DefaultSmallStepFactory>(io_facility, validator);

  auto persistence = std::make_unique<Persistence>();

  auto model_controller = std::make_shared<DefaultModelController>(
      std::move(task_manager), std::move(persistence));

  auto state_machine = std::make_unique<StateMachine>(validator, io_facility,
                                                      small_step_factory);
  StateMachineController ctrl{model_controller, std::move(state_machine)};
  ctrl.Run();

  return 0;
}