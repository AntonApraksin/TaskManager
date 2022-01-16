#include <gtest/gtest.h>

#include "test_utils/ScenarioFramework.h"
#include "test_utils/utils.h"

class DeleteActionTest : public testing::Test, protected ScenarioFramework {
 protected:
  void SetUp() override { return SetUpImpl(); }
};

TEST_F(DeleteActionTest, TaskShouldBeDeleted) {
  auto [storage, _] =
      RunScenario({"add", "fd", "", "", "", "y", "delete 0", "y", "q"});
  EXPECT_EQ(storage.size(), 0);
}

TEST_F(DeleteActionTest, NestedTasksShouldBeDeleted) {
  std::vector<std::string> commands;
  auto [storage, _] = RunScenario(
      {"add", "fdj",   "",     "", "", "y", "add 0", "fsdfs",    "",  "", "",
       "y",   "add 1", "qwqq", "", "", "",  "y",     "delete 0", "y", "q"});
  EXPECT_EQ(storage.size(), 0);
}