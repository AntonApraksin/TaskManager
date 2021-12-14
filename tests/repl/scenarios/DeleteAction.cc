#include "ScenarioFramework.h"

class DeleteActionTest : public testing::Test, protected ScenarioFramework {
 protected:
  void SetUp() override { return SetUpImpl(); }
};

TEST_F(DeleteActionTest, TaskShouldBeDeleted) {
  auto storage =
      RunScenario({"add", "fd", "", "", "", "y", "delete 0", "y", "q"});
  EXPECT_EQ(storage.ShowStorage().size(), 0);
}

TEST_F(DeleteActionTest, NestedTasksShouldBeDeleted) {
  std::vector<std::string> commands;
  auto storage = RunScenario(
      {"add", "fdj",   "",     "", "", "y", "add 0", "fsdfs",    "",  "", "",
       "y",   "add 1", "qwqq", "", "", "",  "y",     "delete 0", "y", "q"});
  EXPECT_EQ(storage.ShowStorage().size(), 0);
}
