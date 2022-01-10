#ifndef TASKMANAGER_TESTS_REPL_TEST_USAGE_USAGEFRAMEWORK_H_
#define TASKMANAGER_TESTS_REPL_TEST_USAGE_USAGEFRAMEWORK_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../common.h"
#include "model/id/TaskIdProducer.h"
#include "model/task_manager/TaskManager.h"
#include "persistence/Persistence.h"
#include "repl/io_facility/IIoFacility.h"
#include "repl/small_steps/default/DefaultSmallStepFactory.h"
#include "repl/state_machine/StateMachineController.h"
#include "repl/validator/DefaultValidator.h"

class UsageMockIoFacility : public IIoFacility {
 public:
  void SetInput(std::vector<std::string> input) { input_ = std::move(input); }

  std::vector<std::string> GetOutput() { return output_; }

  std::string GetLine() override {
    auto to_return = input_.back();
    input_.pop_back();
    return to_return;
  }

  void Print(const std::string& str) override { output_.push_back(str); }

 private:
  std::vector<std::string> input_;
  std::vector<std::string> output_;
};

class UsageFramework {
 protected:
  void SetUpImpl() {
    io_facility_ = std::make_shared<UsageMockIoFacility>();
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
  std::shared_ptr<UsageMockIoFacility> io_facility_;
  std::shared_ptr<ModelController> model_controller_;
  std::shared_ptr<IValidator> validator_;
  TaskStringedDataProducer task_stringed_data_producer_;
};

#endif  // TASKMANAGER_TESTS_REPL_TEST_USAGE_USAGEFRAMEWORK_H_
