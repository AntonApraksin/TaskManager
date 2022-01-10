#ifndef TASKMANAGER_TESTS_REPL_UNIT_STATE_MACHINE_COMMANDS_COMMANDTEST_H_
#define TASKMANAGER_TESTS_REPL_UNIT_STATE_MACHINE_COMMANDS_COMMANDTEST_H_

#include <gtest/gtest.h>

#include "common.h"
#include "model/ModelController.h"
#include "model/id/TaskIdProducer.h"
#include "model/task_manager/TaskManager.h"
#include "persistence/Persistence.h"

class CommandTest : public ::testing::Test {
 public:
  void SetUp() override {
    auto id_producer = std::make_unique<TaskIdProducer>();
    auto task_manager = std::make_unique<TaskManager>(std::move(id_producer));

    auto persistence = std::make_unique<Persistence>();

    model_controller_ = std::make_unique<ModelController>(
        std::move(task_manager), std::move(persistence));
  }

 protected:
  std::unique_ptr<ModelController> model_controller_;
  TaskFactory task_factory_;
};

#endif  // TASKMANAGER_TESTS_REPL_UNIT_STATE_MACHINE_COMMANDS_COMMANDTEST_H_
