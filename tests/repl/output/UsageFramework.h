#ifndef TASKMANAGER_TESTS_REPL_TEST_USAGE_USAGEFRAMEWORK_H_
#define TASKMANAGER_TESTS_REPL_TEST_USAGE_USAGEFRAMEWORK_H_

#if 0

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../common.h"
#include "model/id/TaskIdProducer.h"
#include "model/task_manager/TaskManager.h"
#include "repl/UIController.h"
#include "repl/io_facility/IIoFacility.h"
#include "repl/validator/DefaultValidator.h"
#include "repl/view/steps/iostream/small_step/IostreamSmallStepFactory.h"
#include "repl/view/steps/iostream/step/IostreamStepFactory.h"

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
    task_manager_ = std::make_shared<TaskManager>(std::move(id_producer));

    validator_ = std::make_shared<DefaultValidator>();

    auto small_step_factory =
        std::make_shared<IostreamSmallStepFactory>(io_facility_, validator_);
    auto step_factory = std::make_unique<IostreamStepFactory>(
        io_facility_, validator_, small_step_factory);

    auto view = std::make_unique<View>(io_facility_, validator_);

    controller_ = std::make_unique<UIController>(
        std::move(view), task_manager_, validator_, std::move(step_factory));
  }

  std::pair<TaskStorage, std::vector<std::string>> RunScenario(
      std::vector<std::string> commands) {
    io_facility_->SetInput(
        std::vector<std::string>(commands.crbegin(), commands.crend()));
    controller_->Run();
    return {task_manager_->Show(), io_facility_->GetOutput()};
  }

  Task TaskDataToTask(const TaskStringedData& data) {
    return *CreateTask(data.title, *validator_->ParseTaskDate(data.date),
                       *validator_->ParseTaskPriority(data.priority),
                       *validator_->ParseTaskProgress(data.state));
  }

  std::unique_ptr<UIController> controller_;
  std::shared_ptr<UsageMockIoFacility> io_facility_;
  std::shared_ptr<TaskManager> task_manager_;
  std::shared_ptr<IValidator> validator_;
  TaskStringedDataProducer task_stringed_data_producer_;
};

#endif

#endif  // TASKMANAGER_TESTS_REPL_TEST_USAGE_USAGEFRAMEWORK_H_
