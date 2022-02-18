#include <gtest/gtest.h>

#include <thread>

#include "interactor/state_machine/commands/Commands.h"
#include "model/DefaultModelController.h"
#include "model/task_manager/TaskManager.h"
#include "test_utils/MockPersistence.h"
#include "test_utils/MockTaskIdProducer.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/utils.h"

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

std::unique_ptr<Command> GetRandomCommand() {
  using GetCommandFunctionType = std::unique_ptr<Command> (*)();
  std::unordered_map<int, GetCommandFunctionType> const function_map{
      {0, GetAddTaskCommand},     {1, GetAddSubtaskCommand},
      {2, GetAddLabelCommand},    {3, GetCompleteTaskCommand},
      {4, GetDeleteTaskCommand},  {5, GetDeleteLabelCommand},
      {6, GetGetAllTasksCommand}, {7, GetGetSpecifiedTasksCommand},
      {8, GetEditTaskCommand},
  };
  return (function_map.at(rand() % 9))();
}

}  // namespace task_manager

class DefaultModelControllerDDOSTest : public ::testing::Test {
 protected:
  void SetUp() override {
    auto mtip = std::make_unique<MockTaskIdProducer>();
    auto tm = std::make_unique<TaskManager>(std::move(mtip));
    auto persistence = std::make_unique<MockPersistence>();
    model_controller_ = std::make_unique<DefaultModelController>(
        std::move(tm), std::move(persistence));
  }
  std::unique_ptr<DefaultModelController> model_controller_;
};

void Work(ModelController& model_controller, std::atomic<int>& n_performed,
          int n_commands) {
  for (; n_commands-- != 0;) {
    GetRandomCommand()->execute(model_controller);
    n_performed.fetch_add(1);
  }
}

int RunNCommandsPerThread(ModelController& model_controller,
                          unsigned int n_threads, int n_commands) {
  std::atomic<int> n_performed_commands = 0;
  std::vector<std::thread> threads;
  for (unsigned int i = 0; i != n_threads; ++i) {
    threads.emplace_back(Work, std::ref(model_controller),
                         std::ref(n_performed_commands), n_commands);
  }
  for (auto& i : threads) {
    i.join();
  }
  return n_performed_commands.load();
}

TEST_F(DefaultModelControllerDDOSTest, Run20CommandsPerThread) {
  auto n_threads = std::thread::hardware_concurrency();
  constexpr int kNCommands = 20;
  auto expected =
      RunNCommandsPerThread(*model_controller_, n_threads, kNCommands);
  ASSERT_EQ(expected, n_threads * kNCommands);
}

TEST_F(DefaultModelControllerDDOSTest, Run50CommandsPerThread) {
  auto n_threads = std::thread::hardware_concurrency();
  constexpr int kNCommands = 50;
  auto expected =
      RunNCommandsPerThread(*model_controller_, n_threads, kNCommands);
  ASSERT_EQ(expected, n_threads * kNCommands);
}

TEST_F(DefaultModelControllerDDOSTest, Run100CommandsPerThread) {
  auto n_threads = std::thread::hardware_concurrency();
  constexpr int kNCommands = 100;
  auto expected =
      RunNCommandsPerThread(*model_controller_, n_threads, kNCommands);
  ASSERT_EQ(expected, n_threads * kNCommands);
}

TEST_F(DefaultModelControllerDDOSTest, Run300CommandsPerThread) {
  auto n_threads = std::thread::hardware_concurrency();
  constexpr int kNCommands = 300;
  auto expected =
      RunNCommandsPerThread(*model_controller_, n_threads, kNCommands);
  ASSERT_EQ(expected, n_threads * kNCommands);
}

TEST_F(DefaultModelControllerDDOSTest, Run500CommandsPerThread) {
  auto n_threads = std::thread::hardware_concurrency();
  constexpr int kNCommands = 500;
  auto expected =
      RunNCommandsPerThread(*model_controller_, n_threads, kNCommands);
  ASSERT_EQ(expected, n_threads * kNCommands);
}
