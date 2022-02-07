#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "model/DefaultModelController.h"
#include "model/task_manager/TaskManager.h"
#include "persistence/FilePersistence.h"
#include "service/DefaultTaskService.h"

int main() {
  using namespace task_manager;
  auto id_producer = std::make_unique<TaskIdProducer>();
  auto task_manager = std::make_unique<TaskManager>(std::move(id_producer));

  auto file_persister = std::make_unique<FilePersistence>(
      "very_long_name_that_wont_conflict.txt");

  auto model_controller = std::make_unique<DefaultModelController>(
      std::move(task_manager), std::move(file_persister));

  auto server_address = "0.0.0.0:50051";
  DefaultTaskService service(std::move(model_controller));

  grpc::ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();

  return 0;
}
