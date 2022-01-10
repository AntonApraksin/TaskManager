#ifndef TASKMANAGER_TESTS_REPL_SCENARIOS_SCENARIOFRAMEWORK_H_
#define TASKMANAGER_TESTS_REPL_SCENARIOS_SCENARIOFRAMEWORK_H_

#include <gmock/gmock.h>
#include <google/protobuf/util/time_util.h>
#include <gtest/gtest.h>

#include "../common.h"
#include "model/id/TaskIdProducer.h"
#include "model/task_manager/TaskManager.h"
#include "persistence/Persistence.h"
#include "repl/io_facility/IIoFacility.h"
#include "repl/io_facility/Strings.h"
#include "repl/small_steps/default/DefaultSmallStepFactory.h"
#include "repl/state_machine/StateMachineController.h"
#include "repl/validator/DateFormat.h"
#include "repl/validator/DefaultValidator.h"

class ScenarioMockIoFacility : public IIoFacility {
 public:
  MOCK_METHOD(std::string, GetLine, (), (override));
  MOCK_METHOD(void, Print, (const std::string&), (override));
};

class ScenarioFramework {
 protected:
  void SetUpImpl() {
    io_facility_ = std::make_shared<ScenarioMockIoFacility>();
    auto id_producer = std::make_unique<TaskIdProducer>();
    auto task_manager = std::make_unique<TaskManager>(std::move(id_producer));
    auto persistence = std::make_unique<Persistence>();
    model_controller_ = std::make_shared<ModelController>(
        std::move(task_manager), std::move(persistence));

    validator_ = std::make_shared<DefaultValidator>();

    auto small_step_factory =
        std::make_shared<DefaultSmallStepFactory>(io_facility_, validator_);

    auto state_machine = std::make_unique<StateMachine>(
        validator_, io_facility_, small_step_factory);
    controller_ = std::make_unique<StateMachineController>(
        model_controller_, std::move(state_machine));

    EXPECT_CALL(*io_facility_, Print).Times(testing::AtLeast(1));
  }

  SolidTasks RunScenario(std::vector<std::string> commands) {
    std::vector<std::string> vec(commands.crbegin(), commands.crend());
    auto command_producer = [&vec]() mutable {
      if (vec.empty()) {
        std::logic_error("Vector ran out of commands");
      }
      auto command = vec.back();
      vec.pop_back();
      return command;
    };
    EXPECT_CALL(*io_facility_, GetLine)
        .WillRepeatedly(testing::Invoke(command_producer));
    controller_->Run();
    return model_controller_->GetAllSolidTasks().AccessResult();
  }

  SolidTask TaskDataToSolidTask(const TaskStringedData& data,
                                google::protobuf::int32 id) {
    auto task = *CreateTask(data.title, *validator_->ParseTaskDate(data.date),
                            *validator_->ParseTaskPriority(data.priority),
                            *validator_->ParseTaskProgress(data.state));
    auto task_id = CreateTaskId(id);
    SolidTask solid_task;
    solid_task.set_allocated_task(new Task(std::move(task)));
    solid_task.set_allocated_task_id(new TaskId(task_id));
    return solid_task;
  }

  SolidTask TaskDataToSolidTask(const TaskStringedData& data,
                                google::protobuf::int32 id,
                                google::protobuf::int32 parent_id) {
    auto solid_task = TaskDataToSolidTask(data, id);
    auto parent_task_id = CreateTaskId(parent_id);
    solid_task.set_allocated_parent_id(new TaskId(parent_task_id));
    return solid_task;
  }

  std::unique_ptr<StateMachineController> controller_;
  std::shared_ptr<ScenarioMockIoFacility> io_facility_;
  std::shared_ptr<ModelController> model_controller_;
  std::shared_ptr<IValidator> validator_;
  TaskStringedDataProducer task_stringed_data_producer_;
};

#endif  // TASKMANAGER_TESTS_REPL_SCENARIOS_SCENARIOFRAMEWORK_H_
