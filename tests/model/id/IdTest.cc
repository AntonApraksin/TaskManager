#include <gtest/gtest.h>

#include "model/id/TaskIdProducer.h"
#include "test_utils/common.h"
#include "utils/TaskIdUtils.h"

class IdTest : public ::testing::Test {};

TEST_F(IdTest, TwoDifferentIds)
{
    TaskIdProducer id_prod;

    auto id1 = id_prod.GetNextId();
    auto id2 = id_prod.GetNextId();

    EXPECT_NE(id1, id2);
}

TEST_F(IdTest, TestUniqueness)
{
    constexpr int kElems = 32768;
    TaskIdProducer id_prod;
    std::set<TaskId> task_ids;

    for (int i{0} ; i != kElems ; ++i)
    {
        task_ids.insert(id_prod.GetNextId());
    }

    EXPECT_EQ(task_ids.size(), kElems);
}

TEST_F(IdTest, TestSequencing)
{
    constexpr int kElems = 32768;
    TaskIdProducer tp;

    for (int id{0} ; id != kElems ; ++id)
    {
        EXPECT_EQ(tp.GetNextId().id(), id);
    }
}
