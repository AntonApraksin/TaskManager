#ifndef TASKMANAGER_TESTS_TEST_UTILS_SCENARIOFRAMEWORK_H_
#define TASKMANAGER_TESTS_TEST_UTILS_SCENARIOFRAMEWORK_H_

#include "interactor/io_facility/Strings.h"
#include "interactor/small_steps/default/DefaultSmallStepFactory.h"
#include "interactor/state_machine/StateMachineController.h"
#include "interactor/validator/DefaultValidator.h"
#include "model/DefaultModelController.h"
#include "model/task_manager/TaskManager.h"
#include "persistence/Persistence.h"
#include "test_utils/MockIoFacility.h"
#include "test_utils/MockTaskIdProducer.h"
#include "test_utils/TaskStringedData.h"

class ScenarioFramework {
 protected:
  void SetUpImpl() {
    io_facility_ = std::make_shared<MockIoFacility>();
    auto id_producer = std::make_unique<MockTaskIdProducer>();
    auto task_manager = std::make_unique<TaskManager>(std::move(id_producer));
    model_controller_ =
        std::make_shared<DefaultModelController>(std::move(task_manager));

    validator_ = std::make_shared<DefaultValidator>();

    auto small_step_factory =
        std::make_shared<DefaultSmallStepFactory>(io_facility_, validator_);

    auto state_machine = std::make_unique<StateMachine>(
        validator_, io_facility_, small_step_factory);
    controller_ = std::make_unique<StateMachineController>(
        model_controller_, std::move(state_machine));
  }

  std::pair<SolidTasks, std::vector<std::string>> RunScenario(
      std::vector<std::string> commands) {
    io_facility_->SetInput(
        std::vector<std::string>(commands.crbegin(), commands.crend()));
    controller_->Run();
    return {model_controller_->GetAllSolidTasks().AccessResult(),
            io_facility_->GetOutput()};
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
  std::shared_ptr<MockIoFacility> io_facility_;
  std::shared_ptr<ModelController> model_controller_;
  std::shared_ptr<IValidator> validator_;
  TaskStringedDataProducer task_stringed_data_producer_;
};

#endif  // TASKMANAGER_TESTS_TEST_UTILS_SCENARIOFRAMEWORK_H_
