#include "interactor/io_facility/iostream/IostreamIoFacility.h"
#include "interactor/small_steps/default/DefaultSmallStepFactory.h"
#include "interactor/state_machine/StateMachineController.h"
#include "interactor/validator/DefaultValidator.h"
#include "model/DefaultModelController.h"
#include "model/task_manager/TaskManager.h"
#include "persistence/Persistence.h"

int main() {
  using namespace task_manager;
  auto id_producer = std::make_unique<TaskIdProducer>();
  auto task_manager = std::make_unique<TaskManager>(std::move(id_producer));

  auto validator = std::make_shared<DefaultValidator>();
  auto io_facility = std::make_shared<IostreamIoFacility>();
  auto small_step_factory =
      std::make_shared<DefaultSmallStepFactory>(io_facility, validator);

  auto model_controller =
      std::make_shared<DefaultModelController>(std::move(task_manager));

  auto state_machine = std::make_unique<StateMachine>(validator, io_facility,
                                                      small_step_factory);
  StateMachineController ctrl{model_controller, std::move(state_machine)};
  ctrl.Run();

  return 0;
}