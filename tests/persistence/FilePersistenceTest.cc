#include <gtest/gtest.h>

#include <fstream>

#include "persistence/FilePersistence.h"
#include "test_utils/TaskFactory.h"
#include "test_utils/operators.h"
#include "test_utils/utils.h"

class DISABLED_FilePersistenceTest : public ::testing::Test {};

TEST_F(DISABLED_FilePersistenceTest, SaveAndLoadMustBePerformedInSameOrder)
{
    TaskFactory tf;
    auto _0 = tf.GetNextTask();    // NOLINT: beautiful tree representation
    auto __1 = tf.GetNextTask();   // NOLINT: beautiful tree representation
    auto _2 = tf.GetNextTask();    // NOLINT: beautiful tree representation
    auto __3 = tf.GetNextTask();   // NOLINT: beautiful tree representation
    auto ___4 = tf.GetNextTask();  // NOLINT: beautiful tree representation
    SolidTasks solid_tasks{
        TaskToSolidTask(_0, 0), TaskToSolidTask(_2, 2),
        TaskToSolidTask(__3, 3, 2), TaskToSolidTask(___4, 4, 3),
        TaskToSolidTask(__1, 1, 0),
    };
    FilePersistence persistence(__FILE__ ".txt");
    persistence.Save(solid_tasks);
    auto loaded_solid_tasks = persistence.Load();
    ASSERT_EQ(loaded_solid_tasks.GetStatus(), FilePersistence::Status::kOk);
    ASSERT_EQ(loaded_solid_tasks.AccessResult().size(), solid_tasks.size());
    for (size_t i{0}, sz{solid_tasks.size()} ; i != sz ; ++i)
    {
        EXPECT_EQ(loaded_solid_tasks.AccessResult()[i], solid_tasks[i]);
    }
}

TEST_F(DISABLED_FilePersistenceTest, LoadOnCorruptedDataMustFail)
{
    TaskFactory tf;
    auto _0 = tf.GetNextTask();    // NOLINT: beautiful tree representation
    auto __1 = tf.GetNextTask();   // NOLINT: beautiful tree representation
    auto _2 = tf.GetNextTask();    // NOLINT: beautiful tree representation
    auto __3 = tf.GetNextTask();   // NOLINT: beautiful tree representation
    auto ___4 = tf.GetNextTask();  // NOLINT: beautiful tree representation
    SolidTasks solid_tasks{
        TaskToSolidTask(_0, 0), TaskToSolidTask(_2, 2),
        TaskToSolidTask(__3, 3, 2), TaskToSolidTask(___4, 4, 3),
        TaskToSolidTask(__1, 1, 0),
    };
    FilePersistence persistence(__FILE__ ".txt");
    persistence.Save(solid_tasks);
    std::ofstream to_corrupt(__FILE__ ".txt");
    to_corrupt.write("ffffff", 6);
    to_corrupt.close();
    ASSERT_EQ(persistence.Load().GetStatus(),
              Persistence::Status::kFailureReading);
}