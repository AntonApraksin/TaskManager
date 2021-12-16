#include <gtest/gtest.h>

#include "../common.h"
#include "model/task_manager/TaskManager.h"
#include "persistence/Persistence.h"

class PersistenceTest : public ::testing::Test {};

bool CompareTwoTaskStorages(const TaskStorage& genuine,
                            const TaskStorage& checkable) {
  for (const auto& i : genuine) {
    if (!(*i.second == *checkable.ShowStorage().find(i.first)->second)) {
      return false;
    }
    if (!CompareTwoTaskStorages(
            i.second, checkable.ShowStorage().find(i.first)->second)) {
      return false;
    }
  }
  return true;
}

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
  TaskManager task_manager{std::move(id_producer)};
  auto i0 = *task_manager.Add(t0).first;
  auto i1 = *task_manager.Add(t1).first;
  auto i2 = *task_manager.Add(t2).first;
  auto i00 = *task_manager.Add(i0, t00).first;
  auto i01 = *task_manager.Add(i0, t01).first;
  auto i000 = *task_manager.Add(i00, t000).first;
  auto i0000 = *task_manager.Add(i000, t0000).first;
  auto i10 = *task_manager.Add(i1, t10).first;

  auto genuine_storage = task_manager.Show();
  auto genuine_last_given_id = task_manager.GetLastGivenId();
  Persistence persistence;
  std::stringstream file;
  Payload ipayload{task_manager.GetLastGivenId(), task_manager.Show()};
  auto save_result = persistence.Save(file, ipayload);
  ASSERT_EQ(save_result.status, SaveResult::Status::kOk);

  auto load_result = persistence.Load(file);
  ASSERT_EQ(load_result.status, LoadResult::Status::kOk);
  auto loaded_payload = load_result.payload;
  auto loaded_storage = loaded_payload->task_storage;
  auto loaded_last_given_id = loaded_payload->last_given_id;

  ASSERT_EQ(genuine_last_given_id.id(), loaded_last_given_id.id());

  ASSERT_TRUE(CompareTwoTaskStorages(genuine_storage, loaded_storage));
}