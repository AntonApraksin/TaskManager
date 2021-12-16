#include <gtest/gtest.h>

#include "../common.h"

#include "model/task_manager/TaskManager.h"
#include "persistence/Persistence.h"

class PersistenceTest : public ::testing::Test
{};

TEST_F(PersistenceTest, MakeItWork)
{
  TaskFactory task_factory;
  auto t0 = task_factory.GetNextTask();
  auto t1 = task_factory.GetNextTask();
  auto t2 = task_factory.GetNextTask();
  auto t00 = task_factory.GetNextTask();
  auto t01 = task_factory.GetNextTask();
  auto t000 = task_factory.GetNextTask();
  auto t0000 = task_factory.GetNextTask();
  auto t10 = task_factory.GetNextTask();

  auto id_producer = std::make_unique<TaskIdProducer>();
  TaskManager task_manager{std::move(id_producer)};
  auto i0 = task_manager.Add(t0);
  auto i1 = task_manager.Add(t1);
  auto i2 = task_manager.Add(t2);
  auto i00 = task_manager.Add(i0, t00);
  auto i01 = task_manager.Add(i0, t01);
  auto i000 = task_manager.Add(i00, t000);
  auto i0000 = task_manager.Add(i000, t0000);
  auto i10 = task_manager.Add(i1, t10);

  TaskStorage genuine_storage = task_manager.Show();
  Persistence persistence;
  std::stringstream file;
  auto save_result = persistence.Save(file, task_manager);
  ASSERT_EQ(save_result.status, SaveResult::Status::kOk);
  TaskStorage saved_storage = task_manager.Show();

}