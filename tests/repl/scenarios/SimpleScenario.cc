#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "model/id/TaskIdProducer.h"
#include "model/task_manager/TaskManager.h"
#include "repl/Controller.h"
#include "repl/io_facility/IIoFacility.h"
#include "repl/validator/DefaultValidator.h"
#include "repl/view/steps/iostream/small_step/IostreamSmallStepFactory.h"
#include "repl/view/steps/iostream/step/IostreamStepFactory.h"

using namespace testing;  // NOLINT

bool operator==(const Task& lhs, const Task& rhs) {
  return lhs.GetTitle() == rhs.GetTitle() &&
         lhs.GetPriority() == rhs.GetPriority() &&
         lhs.GetDueDate() == rhs.GetDueDate();
}

class MockIoFacility : public IIoFacility {
 public:
  MOCK_METHOD(std::string, GetLine, (), (override));
  MOCK_METHOD(void, Print, (const std::string&), (override));
};

class ScenariosTest : public ::testing::Test {};

TEST_F(ScenariosTest, TwoAddsTitlesShouldBeEqual) {
  std::string title_1 = "gkldfgkjdsgdsfgd";
  std::string title_2 = "lzfkhgpasjeavfdjs";
  auto io_facility = std::make_shared<MockIoFacility>();
  EXPECT_CALL(*io_facility, GetLine)
      .WillOnce(Return("add"))
      .WillOnce(Return(title_1))
      .WillOnce(Return(""))
      .WillOnce(Return(""))
      .WillOnce(Return(""))
      .WillOnce(Return("y"))
      .WillOnce(Return("add"))
      .WillOnce(Return(title_2))
      .WillOnce(Return(""))
      .WillOnce(Return(""))
      .WillOnce(Return(""))
      .WillOnce(Return("y"))
      .WillOnce(Return("quit"));

  EXPECT_CALL(*io_facility, Print).Times(testing::AtLeast(1));

  auto id_producer = std::make_unique<TaskIdProducer>();
  auto task_manager = std::make_shared<TaskManager>(std::move(id_producer));

  auto validator = std::make_shared<DefaultValidator>();

  auto small_step_factory =
      std::make_shared<IostreamSmallStepFactory>(io_facility, validator);
  auto step_factory = std::make_unique<IostreamStepFactory>(
      io_facility, validator, small_step_factory);

  auto view = std::make_unique<View>(io_facility, validator);

  Controller ctrl{std::move(view), task_manager, std::move(step_factory)};
  ctrl.Run();

  auto storage = task_manager->Show();

  EXPECT_EQ(storage.Find(TaskId::Create(0))->GetTitle(), title_1);
  EXPECT_EQ(storage.Find(TaskId::Create(1))->GetTitle(), title_2);
}

TEST_F(ScenariosTest, TestImeniVladimiraMoiseeva) {
  std::string title = "title";
  std::string date = "12:34 21/07/2021";
  std::string priority = "medium";
  std::string state = "+";

  auto io_facility = std::make_shared<MockIoFacility>();
  EXPECT_CALL(*io_facility, GetLine)
      .WillOnce(Return("hui"))
      .WillOnce(Return("kflsdjfa"))
      .WillOnce(Return("add"))
      .WillOnce(Return(title))
      .WillOnce(Return("322"))
      .WillOnce(Return(date))
      .WillOnce(Return("rhkshg"))
      .WillOnce(Return(priority))
      .WillOnce(Return(";jlgd"))
      .WillOnce(Return("low"))
      .WillOnce(Return(state))
      .WillOnce(Return("y"))
      .WillOnce(Return("q"));

  EXPECT_CALL(*io_facility, Print).Times(testing::AtLeast(1));

  auto id_producer = std::make_unique<TaskIdProducer>();
  auto task_manager = std::make_shared<TaskManager>(std::move(id_producer));

  auto validator = std::make_shared<DefaultValidator>();

  auto small_step_factory =
      std::make_shared<IostreamSmallStepFactory>(io_facility, validator);
  auto step_factory = std::make_unique<IostreamStepFactory>(
      io_facility, validator, small_step_factory);

  auto view = std::make_unique<View>(io_facility, validator);

  Controller ctrl{std::move(view), task_manager, std::move(step_factory)};
  ctrl.Run();

  auto storage = task_manager->Show();

  auto expected_task = Task::Create(
      title, *validator->ParseTaskPriority(priority),
      *validator->ParseTaskDate(date), *validator->ParseTaskState(state));

  EXPECT_EQ(*storage.Find(TaskId::Create(0)), expected_task);
}
