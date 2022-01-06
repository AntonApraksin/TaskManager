#include "model/ModelController.h"

#include <gtest/gtest.h>

#include "../ModelUtils.h"
#include "model/task_manager/TaskManager.h"
#include "persistence/Persistence.h"

using namespace task_manager;

class ModelControllerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    auto mtip = std::make_unique<MockTaskIdProducer>();
    auto tm = std::make_unique<TaskManager>(std::move(mtip));
    auto persistence = std::make_unique<Persistence>();
    model_controller_ = std::make_unique<ModelController>(
        std::move(tm), std::move(persistence));
  }
  std::unique_ptr<ModelController> model_controller_;
  TaskFactory task_factory_;
};

TEST_F(ModelControllerTest, AddMustBePerformed) {
  auto to_add = task_factory_.GetNextTask();
  ASSERT_EQ(model_controller_->Add(to_add).GetStatus(),
            ModelController::Status::kOk);
  auto solid_tasks = model_controller_->GetAllSolidTasks();
  ASSERT_EQ(solid_tasks.GetStatus(), ModelController::Status::kOk);
  EXPECT_EQ(solid_tasks.AccessResult().size(), 1);
  EXPECT_EQ(solid_tasks.AccessResult()[0], TaskToSolidTask(to_add, 0));
}

TEST_F(ModelControllerTest, EditMustBePerformed) {
  auto to_add = task_factory_.GetNextTask();
  auto edited = task_factory_.GetNextTask();
  ASSERT_EQ(model_controller_->Add(to_add).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Edit(CreateTaskId(0), edited).GetStatus(),
            ModelController::Status::kOk);
  auto solid_tasks = model_controller_->GetAllSolidTasks();
  ASSERT_EQ(solid_tasks.GetStatus(), ModelController::Status::kOk);
  EXPECT_EQ(solid_tasks.AccessResult().size(), 1);
  EXPECT_EQ(solid_tasks.AccessResult()[0], TaskToSolidTask(edited, 0));
}

TEST_F(ModelControllerTest, CompleteMustBePerformed) {
  auto to_add = task_factory_.GetNextTask();
  ASSERT_EQ(model_controller_->Add(to_add).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Complete(CreateTaskId(0)).GetStatus(),
            ModelController::Status::kOk);
  auto solid_tasks = model_controller_->GetAllSolidTasks();
  ASSERT_EQ(solid_tasks.GetStatus(), ModelController::Status::kOk);
  EXPECT_EQ(solid_tasks.AccessResult().size(), 1);
  EXPECT_EQ(solid_tasks.AccessResult()[0].task().progress(), Task::kCompleted);
}

TEST_F(ModelControllerTest, DeleteMustBePerformed) {
  auto to_add = task_factory_.GetNextTask();
  ASSERT_EQ(model_controller_->Add(to_add).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Delete(CreateTaskId(0)).GetStatus(),
            ModelController::Status::kOk);
  auto solid_tasks = model_controller_->GetAllSolidTasks();
  ASSERT_EQ(solid_tasks.GetStatus(), ModelController::Status::kOk);
  EXPECT_EQ(solid_tasks.AccessResult().size(), 0);
}

TEST_F(ModelControllerTest, AddSubtaskMustBePerformed) {
  auto to_add = task_factory_.GetNextTask();
  auto to_sub_add = task_factory_.GetNextTask();
  ASSERT_EQ(model_controller_->Add(to_add).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(0), to_sub_add).GetStatus(),
            ModelController::Status::kOk);

  auto solid_tasks = model_controller_->GetAllSolidTasks();
  ASSERT_EQ(solid_tasks.GetStatus(), ModelController::Status::kOk);

  EXPECT_EQ(solid_tasks.AccessResult()[0], TaskToSolidTask(to_add, 0));
  EXPECT_EQ(solid_tasks.AccessResult()[1], TaskToSolidTask(to_sub_add, 1, 0));
}

TEST_F(ModelControllerTest, EditOnUnexistingIdMustResultInNotPresentId) {
  auto to_edit = task_factory_.GetNextTask();
  ASSERT_EQ(model_controller_->Edit(CreateTaskId(0), to_edit).GetStatus(),
            ModelController::Status::kNotPresentId);
  ASSERT_EQ(model_controller_->GetAllSolidTasks().AccessResult().size(), 0);
}

TEST_F(ModelControllerTest, CompleteOnUnexistingIdMustResultInNotPresentId) {
  ASSERT_EQ(model_controller_->Complete(CreateTaskId(0)).GetStatus(),
            ModelController::Status::kNotPresentId);
  ASSERT_EQ(model_controller_->GetAllSolidTasks().AccessResult().size(), 0);
}

TEST_F(ModelControllerTest, DeleteOnUnexistingIdMustResultInNotPresentId) {
  ASSERT_EQ(model_controller_->Delete(CreateTaskId(0)).GetStatus(),
            ModelController::Status::kNotPresentId);
  ASSERT_EQ(model_controller_->GetAllSolidTasks().AccessResult().size(), 0);
}

TEST_F(ModelControllerTest, NestedAddOnUnexistingIdMustResultInNotPresentId) {
  auto to_add = task_factory_.GetNextTask();
  ASSERT_EQ(model_controller_->Add(CreateTaskId(0), to_add).GetStatus(),
            ModelController::Status::kNotPresentId);
  ASSERT_EQ(model_controller_->GetAllSolidTasks().AccessResult().size(), 0);
}

TEST_F(ModelControllerTest, CompleteMustCompleteNestedTasks) {
  auto _0 = task_factory_.GetNextTask();     // NOLINT
  auto __1 = task_factory_.GetNextTask();    // NOLINT
  auto ___3 = task_factory_.GetNextTask();   // NOLINT
  auto ____5 = task_factory_.GetNextTask();  // NOLINT
  auto ___4 = task_factory_.GetNextTask();   // NOLINT
  auto __2 = task_factory_.GetNextTask();    // NOLINT
  ASSERT_EQ(model_controller_->Add(_0).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(0), __1).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(0), __2).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(1), ___3).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(1), ___4).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(3), ____5).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Complete(CreateTaskId(0)).GetStatus(),
            ModelController::Status::kOk);

  auto result = model_controller_->GetAllSolidTasks();
  ASSERT_EQ(result.AccessResult().size(), 6);
  for (const auto& i : result.AccessResult()) {
    EXPECT_EQ(i.task().progress(), Task::kCompleted);
  }
}

TEST_F(ModelControllerTest, DeleteMustDeleteNestedTasks) {
  auto _0 = task_factory_.GetNextTask();     // NOLINT
  auto __1 = task_factory_.GetNextTask();    // NOLINT
  auto ___3 = task_factory_.GetNextTask();   // NOLINT
  auto ____5 = task_factory_.GetNextTask();  // NOLINT
  auto ___4 = task_factory_.GetNextTask();   // NOLINT
  auto __2 = task_factory_.GetNextTask();    // NOLINT
  auto _6 = task_factory_.GetNextTask();     // NOLINT
  ASSERT_EQ(model_controller_->Add(_0).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(0), __1).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(0), __2).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(1), ___3).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(1), ___4).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(3), ____5).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(_6).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Delete(CreateTaskId(0)).GetStatus(),
            ModelController::Status::kOk);

  auto result = model_controller_->GetAllSolidTasks();
  ASSERT_EQ(result.AccessResult().size(), 1);
  EXPECT_EQ(result.AccessResult()[0], TaskToSolidTask(_6, 6));
}

TEST_F(ModelControllerTest, GetAllSolidTasksMustReturnAllSolidTasksSorted) {
  auto _0 = task_factory_.GetNextTask();     // NOLINT
  auto __1 = task_factory_.GetNextTask();    // NOLINT
  auto ___3 = task_factory_.GetNextTask();   // NOLINT
  auto ____7 = task_factory_.GetNextTask();  // NOLINT
  auto ___6 = task_factory_.GetNextTask();   // NOLINT
  auto __2 = task_factory_.GetNextTask();    // NOLINT
  auto _4 = task_factory_.GetNextTask();     // NOLINT
  auto __5 = task_factory_.GetNextTask();    // NOLINT
  auto _8 = task_factory_.GetNextTask();     // NOLINT
  ASSERT_EQ(model_controller_->Add(_0).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(0), __1).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(0), __2).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(1), ___3).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(_4).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(4), __5).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(1), ___6).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(3), ____7).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(_8).GetStatus(),
            ModelController::Status::kOk);

  SolidTasks expected{
      TaskToSolidTask(_8, 8),       TaskToSolidTask(_4, 4),
      TaskToSolidTask(__5, 5, 4),   TaskToSolidTask(_0, 0),
      TaskToSolidTask(__2, 2, 0),   TaskToSolidTask(__1, 1, 0),
      TaskToSolidTask(___6, 6, 1),  TaskToSolidTask(___3, 3, 1),
      TaskToSolidTask(____7, 7, 3),
  };
  auto result = model_controller_->GetAllSolidTasks();
  ASSERT_EQ(result.GetStatus(), ModelController::Status::kOk);
  EXPECT_EQ(result.AccessResult().size(), expected.size());
  for (size_t i{0}, sz{expected.size()}; i != sz; ++i) {
    EXPECT_EQ(result.AccessResult()[i], expected[i]);
  }
}

TEST_F(ModelControllerTest, GetSpecificSolidTasks) {
  auto _0 = task_factory_.GetNextTask();     // NOLINT
  auto __1 = task_factory_.GetNextTask();    // NOLINT
  auto ___3 = task_factory_.GetNextTask();   // NOLINT
  auto ____7 = task_factory_.GetNextTask();  // NOLINT
  auto ___6 = task_factory_.GetNextTask();   // NOLINT
  auto __2 = task_factory_.GetNextTask();    // NOLINT
  auto _4 = task_factory_.GetNextTask();     // NOLINT
  auto __5 = task_factory_.GetNextTask();    // NOLINT
  auto _8 = task_factory_.GetNextTask();     // NOLINT
  ASSERT_EQ(model_controller_->Add(_0).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(0), __1).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(0), __2).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(1), ___3).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(_4).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(4), __5).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(1), ___6).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(CreateTaskId(3), ____7).GetStatus(),
            ModelController::Status::kOk);
  ASSERT_EQ(model_controller_->Add(_8).GetStatus(),
            ModelController::Status::kOk);

  SolidTasks expected{
      TaskToSolidTask(_0, 0),      TaskToSolidTask(__2, 2, 0),
      TaskToSolidTask(__1, 1, 0),  TaskToSolidTask(___6, 6, 1),
      TaskToSolidTask(___3, 3, 1), TaskToSolidTask(____7, 7, 3),
      TaskToSolidTask(_8, 8),
  };

  auto result = model_controller_->GetSpecificSolidTasks(
      {CreateTaskId(0), CreateTaskId(8)});
  ASSERT_EQ(result.GetStatus(), ModelController::Status::kOk);
  EXPECT_EQ(result.AccessResult().size(), expected.size());
  for (size_t i{0}, sz{expected.size()}; i != sz; ++i) {
    EXPECT_EQ(result.AccessResult()[i], expected[i]);
  }
}
