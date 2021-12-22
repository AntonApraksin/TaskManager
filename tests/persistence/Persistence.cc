#include <sstream>

#include <gtest/gtest.h>

#include "persistence/Persistence.h"
#include "../common.h"

class PersistenceTest : public ::testing::Test {};

TEST_F(PersistenceTest, SaveAndLoadMustBePerformedInSameOrder)
{
  TaskFactory tf;
  auto _0 = tf.GetNextTask();
  auto __1 = tf.GetNextTask();
  auto _2 = tf.GetNextTask();
  auto __3 = tf.GetNextTask();
  auto ___4 = tf.GetNextTask();
  SolidTasks solid_tasks
      {
          TaskToSolidTask(_0, 0),
          TaskToSolidTask(_2, 2),
          TaskToSolidTask(__3, 3, 2),
          TaskToSolidTask(___4, 4, 3),
          TaskToSolidTask(__1, 1, 0),
      };
  Persistence persistence;
  std::stringstream ss;
  persistence.Save(ss, solid_tasks);
  auto loaded_solid_tasks = persistence.Load(ss);
  ASSERT_EQ(loaded_solid_tasks.GetStatus(), Persistence::Status::kOk);
  ASSERT_EQ(loaded_solid_tasks.AccessResult().size(), solid_tasks.size());
  for(size_t i{0}, sz{solid_tasks.size()}; i != sz; ++i)
  {
    EXPECT_EQ(loaded_solid_tasks.AccessResult()[i], solid_tasks[i]);
  }
}

/*
TEST_F(PersistenceTest, SaveMustFail){} // TODO: find a way to check this
*/

TEST_F(PersistenceTest, LoadOnCorruptedDataMustFail)
{
  TaskFactory tf;
  auto _0 = tf.GetNextTask();
  auto __1 = tf.GetNextTask();
  auto _2 = tf.GetNextTask();
  auto __3 = tf.GetNextTask();
  auto ___4 = tf.GetNextTask();
  SolidTasks solid_tasks
      {
          TaskToSolidTask(_0, 0),
          TaskToSolidTask(_2, 2),
          TaskToSolidTask(__3, 3, 2),
          TaskToSolidTask(___4, 4, 3),
          TaskToSolidTask(__1, 1, 0),
      };
  Persistence persistence;
  std::stringstream ss;
  persistence.Save(ss, solid_tasks);
  auto str = ss.str();
  str.erase(str.size()-6, str.size()); // Corrupt
  ss.str(str);
  ASSERT_EQ(persistence.Load(ss).GetStatus(), Persistence::Status::kFailure);
}