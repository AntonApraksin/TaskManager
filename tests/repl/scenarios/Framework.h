#ifndef TASKMANAGER_TESTS_REPL_SCENARIOS_FRAMEWORK_H_
#define TASKMANAGER_TESTS_REPL_SCENARIOS_FRAMEWORK_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "model/id/TaskIdProducer.h"
#include "model/task_manager/TaskManager.h"
#include "repl/Controller.h"
#include "repl/io_facility/IIoFacility.h"
#include "repl/validator/DateFormat.h"
#include "repl/validator/DefaultValidator.h"
#include "repl/view/steps/iostream/IostreamStrings.h"
#include "repl/view/steps/iostream/small_step/IostreamSmallStepFactory.h"
#include "repl/view/steps/iostream/step/IostreamStepFactory.h"

// TODO: Create injectable TaskIdProducer with history

inline bool operator==(const Task& lhs,
                       const Task& rhs) {  // TODO: find it a better place
  return lhs.GetTitle() == rhs.GetTitle() &&
         lhs.GetPriority() == rhs.GetPriority() &&
         lhs.GetDueDate() == rhs.GetDueDate();
}

struct TaskStringedData {
  std::string title;
  std::string date;
  std::string priority;
  std::string state;
};

class TaskStringedDataProducer final {
 public:
  TaskStringedData GetData() {
    std::stringstream ss;
    ss << "Sample task #" << state_;
    Date_t chrono_time =
        std::chrono::system_clock::now() + std::chrono::seconds(state_);
    auto time = std::chrono::system_clock::to_time_t(chrono_time);
    Task::Priority priority = static_cast<Task::Priority>(state_ % 3);
    Task::State state = static_cast<Task::State>(state_ % 2);
    ++state_;
    std::stringstream ss_date;
    ss_date << std::put_time(std::localtime(&time), kDatePattern);
    return {ss.str(), ss_date.str(), IostreamStrings::to_string(priority),
            IostreamStrings::to_string(state)};
  }

 private:
  int state_ = 0;
};

class MockIoFacility : public IIoFacility {
 public:
  MOCK_METHOD(std::string, GetLine, (), (override));
  MOCK_METHOD(void, Print, (const std::string&), (override));
};

class Framework {
 protected:
  void SetUpImpl() {
    io_facility_ = std::make_shared<MockIoFacility>();
    auto id_producer = std::make_unique<TaskIdProducer>();
    task_manager_ = std::make_shared<TaskManager>(std::move(id_producer));

    validator_ = std::make_shared<DefaultValidator>();

    auto small_step_factory =
        std::make_shared<IostreamSmallStepFactory>(io_facility_, validator_);
    auto step_factory = std::make_unique<IostreamStepFactory>(
        io_facility_, validator_, small_step_factory);

    auto view = std::make_unique<View>(io_facility_, validator_);

    controller_ = std::make_unique<Controller>(std::move(view), task_manager_,
                                               std::move(step_factory));

    EXPECT_CALL(*io_facility_, Print).Times(testing::AtLeast(1));
  }

  TaskStorage RunScenario(std::vector<std::string> commands) {
    std::vector<std::string> vec(commands.crbegin(), commands.crend());
    auto command_producer = [&vec]() mutable {
      if (vec.empty()) {
        std::logic_error("Vector ran out of commands");
      }
      const auto& command = vec.back();
      vec.pop_back();
      return command;
    };
    EXPECT_CALL(*io_facility_, GetLine)
        .WillRepeatedly(testing::Invoke(command_producer));
    controller_->Run();
    return task_manager_->Show();
  }

  Task TaskDataToTask(const TaskStringedData& data) {
    return *Task::Create(data.title,
                         *validator_->ParseTaskPriority(data.priority),
                         *validator_->ParseTaskDate(data.date),
                         *validator_->ParseTaskState(data.state));
  }

  std::unique_ptr<Controller> controller_;
  std::shared_ptr<MockIoFacility> io_facility_;
  std::shared_ptr<TaskManager> task_manager_;
  std::shared_ptr<IValidator> validator_;
  TaskStringedDataProducer task_stringed_data_producer_;
};

#endif  // TASKMANAGER_TESTS_REPL_SCENARIOS_FRAMEWORK_H_
