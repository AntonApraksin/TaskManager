#include "Model/id.h"

#include <gtest/gtest.h>

bool operator!=(const TaskId& lhs, const TaskId& rhs) { return !(lhs == rhs); }

class IdTest : public ::testing::Test {};

TEST_F(IdTest, JustWorks) {
  TaskIdProducer id_prod;
  auto id1 = id_prod.GetNextId();
  auto id2 = id_prod.GetNextId();
  EXPECT_NE(id1, id2);
}

TEST_F(IdTest, TestUniqueness) {
  constexpr int kElems = 32768;
  TaskIdProducer id_prod;
  std::vector<TaskId> task_ids;
  for (int i{0}; i != kElems; ++i) {
    task_ids.push_back(id_prod.GetNextId());
  }
  std::set<TaskId> set_task_id(task_ids.cbegin(), task_ids.cend());
  EXPECT_EQ(task_ids.size(), set_task_id.size());
  EXPECT_EQ(task_ids.size(), kElems);
}
