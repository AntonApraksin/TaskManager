#include <gtest/gtest.h>

#include "../common.h"
#include "model/ModelController.h"
#include "model/task_manager/TaskManager.h"
#include "persistence/Persistence.h"

class PersistenceTest : public ::testing::Test {};

TEST_F(PersistenceTest, MakeItWork) {
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
  ModelController model_controller{
      std::make_unique<TaskManager>(std::move(id_producer))};

  auto i0 = model_controller.Add(t0).AccessResult();
  auto i1 = model_controller.Add(t1).AccessResult();
  auto i2 = model_controller.Add(t2).AccessResult();
  auto i00 = model_controller.Add(i0, t00).AccessResult();
  auto i01 = model_controller.Add(i0, t01).AccessResult();
  auto i000 = model_controller.Add(i00, t000).AccessResult();
  auto i0000 = model_controller.Add(i000, t0000).AccessResult();
  auto i10 = model_controller.Add(i1, t10).AccessResult();

  auto genuine_solid_tasks = model_controller.GetAllSolidTasks().AccessResult();
  Persistence persistence;
  std::ostringstream ofile;
  auto save_result = persistence.Save(ofile, genuine_solid_tasks);
  ASSERT_EQ(save_result.GetStatus(), Persistence::Status::kOk);


  std::istringstream ifile(ofile.str());
  auto load_result = persistence.Load(ifile);
  ASSERT_EQ(load_result.GetStatus(), Persistence::Status::kOk);
  auto loaded_solid_tasks = load_result.AccessResult();
  ASSERT_EQ(genuine_solid_tasks.size(), loaded_solid_tasks.size());
  for (size_t i{0}, sz{genuine_solid_tasks.size()}; i != sz; ++i) {
    EXPECT_EQ(genuine_solid_tasks[i].task(), loaded_solid_tasks[i].task());
    EXPECT_EQ(genuine_solid_tasks[i].task_id(),
              loaded_solid_tasks[i].task_id());
    if (genuine_solid_tasks[i].has_parent_id()) {
      EXPECT_EQ(genuine_solid_tasks[i].parent_id(),
                loaded_solid_tasks[i].parent_id());
    }
  }
}