#include "client/TaskServiceModelController.h"
#include "interactor/io_facility/iostream/IostreamIoFacility.h"
#include "interactor/small_steps/default/DefaultSmallStepFactory.h"
#include "interactor/state_machine/StateMachineController.h"
#include "interactor/validator/DefaultValidator.h"
#include "logging/DefaultLogFacility.h"

int main() {
  using namespace task_manager;

  {
    auto format = "[%TimeStamp%][%Severity%](%Scope%): %Message%";
    logging::SetUp();
    logging::CreateFileLog("client", format, logging::severinity::debug);
  }

  auto validator = std::make_shared<DefaultValidator>();
  auto io_facility = std::make_shared<IostreamIoFacility>();
  auto small_step_factory =
      std::make_shared<DefaultSmallStepFactory>(io_facility, validator);

  auto state_machine = std::make_unique<StateMachine>(validator, io_facility,
                                                      small_step_factory);

  std::string server_addr{"127.0.0.1:50051"};
  auto stub = std::make_unique<TaskService::Stub>(
      grpc::CreateChannel(server_addr, grpc::InsecureChannelCredentials()));
  auto task_service_model_controller =
      std::make_shared<TaskServiceModelController>(std::move(stub));
  StateMachineController ctrl{task_service_model_controller,
                              std::move(state_machine)};
  ctrl.Run();

  return 0;
}
