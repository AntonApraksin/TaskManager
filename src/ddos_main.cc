#include <thread>

#include "client/TaskServiceModelController.h"
#include "interactor/state_machine/commands/Commands.h"
#include "logging/DefaultLogFacility.h"
#include "utils/TaskIdUtils.h"
#include "utils/TaskUtils.h"

namespace task_manager {

std::unique_ptr<Command> GetAddLabelCommand() {
  Label label;
  label.set_name("label");
  auto id = CreateTaskId(std::rand() % 512);
  return std::make_unique<AddLabelCommand>(std::move(id), std::move(label));
}

std::unique_ptr<Command> GetDeleteLabelCommand() {
  Label label;
  label.set_name("label");
  auto id = CreateTaskId(std::rand() % 512);
  return std::make_unique<AddLabelCommand>(std::move(id), std::move(label));
}

std::unique_ptr<Command> GetAddTaskCommand() {
  auto task = CreateTask("task", {}, Task::kHigh, Task::kCompleted);
  return std::make_unique<AddTaskCommand>(
      std::move(*task));  // intentional unchecked access
}

std::unique_ptr<Command> GetAddSubtaskCommand() {
  auto task = CreateTask("task", {}, Task::kHigh, Task::kCompleted);
  auto id = CreateTaskId(std::rand() % 512);
  return std::make_unique<AddSubtaskCommand>(
      std::move(id), std::move(*task));  // intentional unchecked access
}

std::unique_ptr<Command> GetEditTaskCommand() {
  auto task = CreateTask("task", {}, Task::kHigh, Task::kCompleted);
  auto id = CreateTaskId(std::rand() % 512);
  return std::make_unique<EditTaskCommand>(
      std::move(id), std::move(*task));  // intentional unchecked access
}

std::unique_ptr<Command> GetCompleteTaskCommand() {
  auto id = CreateTaskId(std::rand() % 512);
  return std::make_unique<CompleteTasksCommand>(
      std::vector<TaskId>{std::move(id)});
}

std::unique_ptr<Command> GetDeleteTaskCommand() {
  auto id = CreateTaskId(std::rand() % 512);
  return std::make_unique<DeleteTasksCommand>(
      std::vector<TaskId>{std::move(id)});
}

std::unique_ptr<Command> GetGetAllTasksCommand() {
  return std::make_unique<GetAllTasksCommand>();
}

std::unique_ptr<Command> GetGetSpecifiedTasksCommand() {
  std::vector<TaskId> ids{
      CreateTaskId(std::rand() % 512), CreateTaskId(std::rand() % 512),
      CreateTaskId(std::rand() % 512), CreateTaskId(std::rand() % 512)};
  return std::make_unique<GetSpecifiedTasksCommand>(std::move(ids));
}

std::unique_ptr<Command> GetSaveTasksCommand() {
  return std::make_unique<SaveTasksCommand>();
}

std::unique_ptr<Command> GetLoadTasksCommand() {
  return std::make_unique<LoadTasksCommand>();
}

std::unique_ptr<Command> GetRandomCommand() {
  using GetCommandFunctionType = std::unique_ptr<Command> (*)();
  std::unordered_map<int, GetCommandFunctionType> const function_map{
      {0, GetAddTaskCommand},     {1, GetAddSubtaskCommand},
      {2, GetAddLabelCommand},    {3, GetCompleteTaskCommand},
      {4, GetDeleteTaskCommand},  {5, GetDeleteLabelCommand},
      {6, GetGetAllTasksCommand}, {7, GetGetSpecifiedTasksCommand},
      {8, GetEditTaskCommand},    {9, GetLoadTasksCommand},
      {10, GetSaveTasksCommand},
  };
  return (function_map.at(rand() % 11))();
}

constexpr int kNCommands = 2000;

void Worker(ModelController& controller) {
  for (int i = 0; i != kNCommands; ++i) {
    GetRandomCommand()->execute(controller);
  }
}
}  // namespace task_manager

int main() {
  using namespace task_manager;
  std::srand(std::time(nullptr));
  std::string server_address = "127.0.0.1:50051";

  auto stub = std::make_unique<TaskService::Stub>(
      grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));
  TaskServiceModelController task_service_model_controller(std::move(stub));

  std::vector<std::thread> threads;
  for (unsigned int i = 0; i != std::thread::hardware_concurrency(); ++i) {
    threads.emplace_back(Worker, std::ref(task_service_model_controller));
  }

  for (auto& i : threads) {
    i.join();
  }

  return 0;
}